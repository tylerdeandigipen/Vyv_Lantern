//------------------------------------------------------------------------------
//
// File Name:	LaserSystem.h
// Author(s):	Thomas Stephenson
// Purpose:		LaserSystem class declaration for managing laser emitters and
//              reflectors.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#if !defined(LASER_SYSTEM_H)
#define LASER_SYSTEM_H

//#include "Engine.h"
#include "BaseSystem.h"
#include "Vector.h"

struct laser_emitter
{
    Vector2 Position;
    Vector2 Direction;

	uint32_t FirstPathNodeIndex;
	uint32_t OnePastLastPathNodeIndex;
};

struct reflector
{
    Vector2 Position;
    Vector2 Direction;
    float Radius;
};

struct beam_path_node
{
    Vector2 Position;
    bool AtInfinity;
};

struct emitter_id
{
    uint32_t Value;
};

struct reflector_id
{
    uint32_t Value;
};

class BaseSystem;
class Logging;
class Inputs;

class LaserSystem : public BaseSystem
{
public:
	struct beam_path_iterator
	{
		laser_emitter* Emitter;
		beam_path_node* CurrentNode;
		uint32_t CurrentNodeIndex;

		bool IsValid(void)
		{
			bool Result = false;
			if(Emitter)
			{
				Result = (CurrentNodeIndex < Emitter->OnePastLastPathNodeIndex);
			}

			return(Result);
		}

		void Advance(void)
		{
			++CurrentNodeIndex;
			CurrentNode = LaserSystem::GetInstance()->GetPathNode(CurrentNodeIndex);
		}
	};

public:

    static LaserSystem *GetInstance(void);
    
    LaserSystem(void);
    virtual ~LaserSystem(void);
    
	Engine::EngineCode Init();
	Engine::EngineCode Close();
	void Update(float dt);
	void Render();

	void HandleSceneChange(void);

	beam_path_iterator IterateEmitterPath(emitter_id Emitter);
	
	emitter_id CreateEmitter(void);
    laser_emitter *GetEmitter(emitter_id ID);

    reflector_id CreateReflector(void);
    reflector *GetReflector(reflector_id ID);
    
private:

    bool DEBUGKeyPressed(int KeyCode, Inputs *InputManager);
    
#define MAX_LASER_EMITTERS 256
	laser_emitter Emitters[MAX_LASER_EMITTERS];
	uint32_t EmitterCount;

#define MAX_REFLECTORS 256
    reflector Reflectors[MAX_REFLECTORS];
    uint32_t ReflectorCount;

    Logging *ErrorLog;

    int DEBUGPreviousScancode;
    bool DEBUGVisualizeReflectorBounces;

    beam_path_node BeamPathNodes[4096];
    uint32_t PathNodeCount;

	beam_path_node *GetPathNode(uint32_t NodeIndex);
};

#endif
