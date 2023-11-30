#include "BehaviorMirror.h"
#include "Collider.h"
#include "Entity.h"
#include "Inputs.h"
#include "Transform.h"
#include "Physics.h"
#include "Renderer.h"
#include "LevelBuilder.h"
#include "Maths/Vector.h"
#include "AudioEngine.h"
#include "Collision.h"
#include <cassert>
#include <cmath>
#include <iostream>



BehaviorMirror::BehaviorMirror() : Behavior(Behavior::Mirror)
{
    _type = this;
}

BehaviorMirror::BehaviorMirror(BehaviorMirror const& other) : Behavior(other)
{
    _type = this;
}

BehaviorMirror::~BehaviorMirror()
{
}

std::string BehaviorMirror::GetName()
{
    return Name();
}

std::string BehaviorMirror::Name()
{
    return "BehaviorMirror";
}

void BehaviorMirror::SetInputHandler(Inputs* _input)
{
}

void BehaviorMirror::Init()
{


    if (Parent())
    {
        // Set collision handler for mirrors with lasers maybe??
        Collider* collider = Parent()->Has(Collider);
    }
}

Behavior* BehaviorMirror::Clone() const
{
    return new BehaviorMirror(*this);
}

void BehaviorMirror::Update(float dt)
{
    if (Parent())
        Controller(dt);
}

void BehaviorMirror::Read(json jsonData)
{
    Init();
}

void BehaviorMirror::SwitchOn(bool collided)
{
    if (collided == true)
    {
        /*
        const EntityContainer entityContainer = *LevelBuilder::GetInstance()->GetContainer();
        int numEntities = LevelBuilder::GetInstance()->CountEntities();

            for (int i = 0; i < numEntities; i++)
            {
                Entity* entity = entityContainer[i];
                if (entity)
                {
                }
            }
            */
    }
}

void BehaviorMirror::MirrorCollisionHandler(Entity* thisone, Entity* other)
{
}

void BehaviorMirror::Controller(float dt)
{
    // DO MIRROR THINGS HERE
}