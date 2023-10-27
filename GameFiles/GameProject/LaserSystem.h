#if !defined(LASER_SYSTEM_H)
#define LASER_SYSTEM_H

#include "Engine.h"

struct laser_emitter
{
    Vector2 P;
    Vector2 Direction;
};

struct reflector
{
    Vector2 P;
    Vector2 Direction;
    float Radius;
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

class LaserSystem : public BaseSystem
{
public:

    static LaserSystem *GetInstance(void);
    
    LaserSystem(void);
    virtual ~LaserSystem(void);
    
	Engine::EngineCode Init();
	Engine::EngineCode Close();
	void Update(float dt);
	void Render();

    emitter_id CreateEmitter(void);
    laser_emitter *GetEmitter(emitter_id ID);

    reflector_id CreateReflector(void);
    reflector *GetReflector(reflector_id ID);
    
private:
    
#define MAX_LASER_EMITTERS 64
	laser_emitter Emitters[MAX_LASER_EMITTERS];
	uint32_t EmitterCount;

#define MAX_REFLECTORS 64
    reflector Reflectors[MAX_REFLECTORS];
    uint32_t ReflectorCount;
};


#endif
