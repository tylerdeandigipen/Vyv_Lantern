//------------------------------------------------------------------------------
//
// File Name:	BehaviorMirror.cpp
// Author(s):	Louis Wang, Michael Howard
// Purpose:		Implementation of BehaviorMirror class for controlling mirror 
//              entities.
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "BehaviorMirror.h"
#include "Collider.h"
#include "Entity.h"
#include "Inputs.h"
#include "Transform.h"
#include "Physics.h"
#include "Renderer.h"
#include "EntityContainer.h"
#include "Maths/Vector.h"
#include "AudioEngine.h"
#include "Collision.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include "EntityContainer.h"
#include "BehaviorSwitch.h"

int BehaviorMirror::count = 1;
int BehaviorMirror::maxCount = 4;
//gfxVector2 BehaviorMirror::currentPos = gfxVector2(0, 0);
//gfxVector2 BehaviorMirror::targetPos = gfxVector2(0, 0);

static reflector* NewReflector(void)
{
    reflector* Result = NULL;

    reflector_id ResultID = LaserSystem::GetInstance()->CreateReflector();
    Result = LaserSystem::GetInstance()->GetReflector(ResultID);

    return(Result);
}

BehaviorMirror::BehaviorMirror() : Behavior(Behavior::Mirror), reflect(NewReflector()), pos()
{
    _type = this;
}

BehaviorMirror::BehaviorMirror(BehaviorMirror const& other) : Behavior(other), reflect(other.reflect), pos(other.pos)
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
        if (Parent() && Parent()->Has(Transform)) {
            //Parent()->GetImage()->position = currentPos;
        }
    }
}

Behavior* BehaviorMirror::Clone() const
{
    return new BehaviorMirror(*this);
}

void BehaviorMirror::Update(float dt)
{
    if (Parent() && Parent()->Has(Transform)) {
        Vector2 position = *Parent()->Has(Transform)->GetTranslation();
        reflect->Position.x = position.x + 4.0f;  // offset to center of mirror
        reflect->Position.y = position.y + 12.0f; // offset to center of mirror
    }
}

void BehaviorMirror::Read(json jsonData)
{
    Init();

    if (jsonData["Direction"].is_object())
    {
        json direction = jsonData["Direction"];
        gfxVector2 angle;
        angle.x = direction["DirectionX"];
        angle.y = direction["DirectionY"];
        reflect->Direction = Vector2::Normalize(angle);
    }

    reflect->Radius = jsonData["Radius"];

    for (auto& positions : jsonData["pos"])
    {
        // Extract "x" and "y" values, convert them to integers, and store in the vector
        Vector2 position{};
        position.x = std::stoi(positions["x"].get<std::string>());
        position.y = std::stoi(positions["y"].get<std::string>());
        pos.push_back(position);
    }
    key = jsonData["key"];
    //currentPos = pos[0];
    //targetPos = pos[0];
}

void BehaviorMirror::SwitchOn(bool collided)
{
    if (collided == true)
    {
        EntityContainer* entityContainer = EntityContainer::GetInstance();
        int numEntities = entityContainer->CountEntities();

        for (int i = 0; i < numEntities; i++) {
            Entity* entity = (*entityContainer)[i];
            BehaviorMirror* mirror = reinterpret_cast<BehaviorMirror*>(entity->Has(Behavior));
            BehaviorSwitch* Switch = reinterpret_cast<BehaviorSwitch*>(entity->Has(Behavior));
            

        }
    }
}

void BehaviorMirror::MirrorCollisionHandler(Entity* thisone, Entity* other)
{
}

void BehaviorMirror::Controller(float dt)
{

 //   float lerpFactor = 0.5f; // Adjust this value for speed

//    currentPos = currentPos + lerpFactor * (targetPos - currentPos);

 //   if (Parent() && Parent()->Has(Transform)) {
 //       Parent()->GetImage()->position = currentPos;
 //   }
}