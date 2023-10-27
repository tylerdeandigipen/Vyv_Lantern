#include <stdint.h>
#include "Vector.h"
#include "BaseSystem.h"
#include "LaserSystem.h"
#include "Renderer.h"

static LaserSystem GlobalInstance;

LaserSystem *LaserSystem::GetInstance(void)
{
    return(&GlobalInstance);
}

LaserSystem::LaserSystem(void)
    :
    BaseSystem("LaserSystem")
{
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

void LaserSystem::Update(float dt)
{
    
}

void LaserSystem::Render(void)
{
    // @NOTE: Laser test
	float MaxLaserTravelDistance = 100000.0f;
    reflector *PreviousReflector = NULL;
    
	for(uint32_t EmitterIndex = 0;
		EmitterIndex < EmitterCount;
		++EmitterIndex)
	{
		laser_emitter *Emitter = Emitters + EmitterIndex;

        int BounceCount = 0;
        Vector2 RayP = Emitter->P;
        Vector2 RayD = Emitter->Direction;

        for(uint32_t IterationCount = 1000;
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

            if(HitReflector)
            {
#if 1
                // @NOTE: Illuminate the line from the previous reflector to the current hit
                if(PreviousReflector)
                {
                    Renderer::GetInstance()->DrawLine(PreviousReflector->P,
                                                      HitReflector->P,
                                                      Color(0x00, 0x00, 0xff, 0xff));
                }
#endif
                RayP = HitPoint;
                RayD -= 2.0f*Vector2::DotProduct(HitReflector->Direction, RayD)*HitReflector->Direction;
                RayD = Vector2::Normalize(RayD);
                ++BounceCount;
            }
            else
            {
                // @NOTE: No other more reflectors face this ray
                break;
            }

            PreviousReflector = HitReflector;
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
	}
    
    return(Result);
}
