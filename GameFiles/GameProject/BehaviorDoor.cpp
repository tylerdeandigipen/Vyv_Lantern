
//------------------------------------------------------------------------------
//
// File Name:	BehaviorPlayer.cpp
// Author(s):	Michael Howard (michael.howard)
// Purpose:		The players behavior
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "AudioEngine.h"
#include "BehaviorDoor.h"
#include "Entity.h"
#include "Collider.h"
#include "Inputs.h"
#include "LevelBuilder.h"
#include "Maths/Vector.h"
#include "Physics.h"
#include "Renderer.h"
#include "Transform.h"

BehaviorDoor::BehaviorDoor() : Behavior(Behavior::Door), isDoorClosed(false)
{
    _type = this;
}

BehaviorDoor::BehaviorDoor(BehaviorDoor const& other) : Behavior(other), isDoorClosed(other.isDoorClosed)
{
    _type = this;
}

BehaviorDoor::~BehaviorDoor()
{
}

Behavior* BehaviorDoor::Clone() const
{
    return new BehaviorDoor(*this);
}

void BehaviorDoor::Init()
{
    if (Parent())
    {
        // always make the behavior after the collider to avoid problems
        Collider* collider = Parent()->Has(Collider);
        if (collider)
        {
            collider->SetCollisionHandler(DoorCollisionHandler);
        }
    }
}

std::string BehaviorDoor::GetName()
{
    return Name();
}

std::string BehaviorDoor::Name()
{
    return "BehaviorDoor";
}

void BehaviorDoor::Update(float dt)
{
}


void BehaviorDoor::Read(json jsonData)
{
    Init();
}

void BehaviorDoor::DoorCollisionHandler(Entity* entity1, Entity* entity2)
{
    // check the player the door
    if (entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Door") == 0)
    {
        // use schillings finite state machine example to implement a set door closed 
        //entity2->Has(Behavior)->se
    }
    if (entity1->GetRealName().compare("Door") == 0 && entity2->GetRealName().compare("Player") == 0)
    {

    }
}

bool BehaviorDoor::GetDoorClosed()
{
    return isDoorClosed;
}

void BehaviorDoor::SetDoorClosed()
{
    isDoorClosed = true;
}
