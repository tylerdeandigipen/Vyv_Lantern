#include "BehaviorSwitch.h"
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

BehaviorSwitch::BehaviorSwitch() : Behavior(Behavior::Switch)
{
	_type = this;
}

BehaviorSwitch::BehaviorSwitch(BehaviorSwitch const& other) : Behavior(other)
{
	_type = this;
}

BehaviorSwitch::~BehaviorSwitch()
{
}

std::string BehaviorSwitch::GetName()
{
	return Name();
}

std::string BehaviorSwitch::Name()
{
	return "BehaviorSwitch";
}

void BehaviorSwitch::SetInputHandler(Inputs* _input)
{
}

void BehaviorSwitch::Init()
{
    if (Parent())
    {
        // Set collision handler for switches
        Collider* collider = Parent()->Has(Collider);
        if (collider)
        {
            collider->SetCollisionHandler(SwitchCollisionHandler);
        }
    }
}

Behavior* BehaviorSwitch::Clone() const
{
    return new BehaviorSwitch(*this);
}

void BehaviorSwitch::Update(float dt)
{
    if (Parent())
        Controller(dt);
}

void BehaviorSwitch::Read(json jsonData)
{
    Init();
    /*What values to load into the switches here*/
}

void BehaviorSwitch::SwitchCollisionHandler(Entity* entity1, Entity* entity2)
{
    if (entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Switch") == 0 || entity1->GetRealName().compare("Switch") == 0 && entity2->GetRealName().compare("Player") == 0)
    {
        /*Check if player is inside switch*/
        int colliding = CollisionCheck(entity1->GetImage()->aabb, entity2->GetImage()->aabb);
    }
}



void BehaviorSwitch::Controller(float dt)
{
    // DO SWITCH THINGS HERE
}
