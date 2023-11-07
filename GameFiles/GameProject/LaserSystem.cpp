#include <stdint.h>
#include "stdafx.h"
#include "Inputs.h"
#include "Vector.h"
#include "BaseSystem.h"
#include "LaserSystem.h"
#include "Renderer.h"
#include "Logging.h"

static LaserSystem GlobalInstance;

LaserSystem *LaserSystem::GetInstance(void)
{
    return(&GlobalInstance);
}

LaserSystem::LaserSystem(void)
    :
    BaseSystem("LaserSystem"),
	DEBUGVisualizeReflectorBounces(false)
{
    ErrorLog = &Logging::GetInstance("debugLog.log");

    PathNodeCount = 0;
    EmitterCount = 0;
    ReflectorCount = 0;
}

LaserSystem::~LaserSystem(void)
{
}

Engine::EngineCode LaserSystem::Close(void)
{
    Engine::EngineCode Result = Engine::NothingBad;
    return(Result);
}

Engine::EngineCode LaserSystem::Init(void)
{
    Engine::EngineCode Result = Engine::NothingBad;
    
    return(Result);
}

bool LaserSystem::DEBUGKeyPressed(int KeyCode, Inputs *InputManager)
{
    // @TODO: Add support for querying key presses in the input manager
    bool Result = false;
    if((DEBUGPreviousScancode != KeyCode) && InputManager->keyPressed(KeyCode))
    {
        Result = true;
        DEBUGPreviousScancode = KeyCode;
    }

    return(Result);
}

void LaserSystem::Update(float dt)
{
    static Inputs *InputManager = Inputs::GetInstance();

    if(InputManager->keyPressed(SDL_SCANCODE_LCTRL))
    {
        if(DEBUGKeyPressed(SDL_SCANCODE_B, InputManager))
        {
			DEBUGVisualizeReflectorBounces = !DEBUGVisualizeReflectorBounces;
        }
        else
        {
            DEBUGPreviousScancode = 0;
        }
    }

    PathNodeCount = 0;
    
	float MaxLaserTravelDistance = 100000.0f;
    reflector *PreviousReflector = NULL;
    
    for(uint32_t EmitterIndex = 0;
		EmitterIndex < EmitterCount;
		++EmitterIndex)
	{
		laser_emitter *Emitter = Emitters + EmitterIndex;

        Emitter->FirstPathNodeIndex = PathNodeCount;
        
        int BounceCount = 0;
        Vector2 RayP = Emitter->P;
        Vector2 RayD = Emitter->Direction;

        for(uint32_t IterationCount = 1024;
			IterationCount;
            --IterationCount)
        {
            reflector *HitReflector = NULL;
            float MinDistance = MaxLaserTravelDistance;
            
            for(uint32_t ReflectorIndex = 0;
                ReflectorIndex < ReflectorCount;
                ++ReflectorIndex)
            {
                reflector *Reflector = Reflectors + ReflectorIndex;

                // @NOTE: Only test reflectors that face against the ray's direction
                float Denom = Vector2::DotProduct(Reflector->Direction, RayD);
                
                if(Denom < 0.0f)
                {
                    float t = Vector2::DotProduct(Reflector->P - RayP, Reflector->Direction) / Denom;
                    if((t > 0.0f) && (t < MinDistance))
                    {
                        // @NOTE: Ensure the ray actually intersectst the plane 
                        Vector2 IntersectionP = RayP + t*RayD;
                        float k = Vector2::DotProduct(Reflector->P - IntersectionP, Vector2::Perp(Reflector->Direction));
                        if(fabsf(k) <= Reflector->Radius)
                        {
                            MinDistance = t;
                            HitReflector = Reflector;
                        }
                    }
                }
            }
        
            Vector2 HitPoint = RayP + MinDistance*RayD;
            Renderer::GetInstance()->DrawLine(RayP, HitPoint, Color(255, 255, 255, 255));

            beam_path_node *NewNode = BeamPathNodes + PathNodeCount++;
            NewNode->P = HitPoint;
            NewNode->AtInfinity = false;
            
            if(HitReflector)
            {
                if(DEBUGVisualizeReflectorBounces)
                {
                    // @NOTE: Illuminate the line from the previous reflector to the current hit
                    if(PreviousReflector)
                    {
                        Renderer::GetInstance()->DrawLine(PreviousReflector->P,
                                                          HitReflector->P,
                                                          Color(0x00, 0x00, 0xff, 0xff));
                    }
                }

                RayP = HitPoint;
                RayD -= 2.0f*Vector2::DotProduct(HitReflector->Direction, RayD)*HitReflector->Direction;
                RayD = Vector2::Normalize(RayD);
                ++BounceCount;
            }
            else
            {
                // @NOTE: No more reflectors face this ray
                NewNode->AtInfinity = true;
                break;
            }

            PreviousReflector = HitReflector;
        }

        Emitter->OnePastLastPathNodeIndex = PathNodeCount;
	}
}

void LaserSystem::Render(void)
{
    for(uint32_t EmitterIndex = 0;
        EmitterIndex < EmitterCount;
        ++EmitterIndex)
    {
        laser_emitter *Emitter = Emitters + EmitterIndex;

        beam_path_node *LastNode = NULL;
        for(uint32_t NodeIndex = Emitter->FirstPathNodeIndex;
            NodeIndex < Emitter->OnePastLastPathNodeIndex;
            ++NodeIndex)
        {
            beam_path_node *PathNode = BeamPathNodes + NodeIndex;

            Vector2 LineBeginP = Emitter->P;
            if(LastNode)
            {
                LineBeginP = LastNode->P;
            }
            
            Renderer::GetInstance()->DrawLine(PathNode->P, LineBeginP, Color(0xff, 0xff, 0xff, 0xff));
            LastNode = PathNode;
        }
    }
    
    for(uint32_t ReflectorIndex = 0;
        ReflectorIndex < ReflectorCount;
        ++ReflectorIndex)
    {
        reflector *Reflector = Reflectors + ReflectorIndex;

        Vector2 ReflectorP = Reflector->P;
        Vector2 Tangent = Vector2::Perp(Reflector->Direction);
        Vector2 StartP = ReflectorP - Reflector->Radius*Tangent;
        Vector2 EndP = ReflectorP + Reflector->Radius*Tangent;

        Renderer::GetInstance()->DrawLine(StartP, EndP, Color(0xff, 0x00, 0x00, 0xff));

        Renderer::GetInstance()->DrawLine(ReflectorP, ReflectorP + 10.0f*Reflector->Direction,
                                          Color(0x00, 0xff, 0x00, 0xff));
    }
}

laser_emitter *LaserSystem::GetEmitter(emitter_id ID)
{
    laser_emitter *Result = NULL;
    if(ID.Value < MAX_LASER_EMITTERS)
    {
        Result = Emitters + ID.Value;
    }
    else
    {
        // TODO(thomas): Logging!
        ErrorLog->Log("GetEmitter was passed an inavalid emitter ID of %i.", ID.Value);
    }

    return(Result);
}

emitter_id LaserSystem::CreateEmitter(void)
{
    emitter_id Result = {0};
    
    if(EmitterCount < MAX_LASER_EMITTERS)
    {
        // TODO(thomas): Recylce old lasers
		uint32_t NewEmitterIndex = EmitterCount++;
		Result.Value = NewEmitterIndex;
	}
    else
    {
		Result.Value = 0xffffffff; // NOTE(thomas): Arbirtarily invalid laser index
        ErrorLog->Log("No more emitters can be created at this time.");
	}
    
    return(Result);
}


reflector *LaserSystem::GetReflector(reflector_id ID)
{
    reflector *Result = NULL;
    if(ID.Value < MAX_REFLECTORS)
    {
        Result = Reflectors + ID.Value;
    }
    else
    {
        // TODO(thomas): Logging!
        ErrorLog->Log("GetReflector was passed an inavalid reflector ID of %i.", ID.Value);
    }

    return(Result);
}

reflector_id LaserSystem::CreateReflector(void)
{
    reflector_id Result = {0};
    
    if(ReflectorCount < MAX_LASER_EMITTERS)
    {
        // TODO(thomas): Recylce old indicies
		uint32_t NewIndex = ReflectorCount++;
		Result.Value = NewIndex;
    }
    else
    {
		Result.Value = 0xffffffff; // NOTE(thomas): Arbirtarily invalid index
        ErrorLog->Log("No more reflectors can be created at this time.");
	}
    
    return(Result);
}
