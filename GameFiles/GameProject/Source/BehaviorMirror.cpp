//------------------------------------------------------------------------------
//
// File Name:	BehaviorMirror.cpp
// Author(s):	Louis Wang, Michael Howard
// Purpose:		Implementation of BehaviorMirror class for controlling mirror 
//              entities.
// 
// Copyright ?2023 DigiPen (USA) Corporation.
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

BehaviorMirror::BehaviorMirror() : Behavior(Behavior::bMirror), mirror(), pos()
{
    key = 0;
    mirror = new Mirror;
    _type = this; // i dont know what this is
}

BehaviorMirror::BehaviorMirror(BehaviorMirror const& other) : Behavior(other), mirror(other.mirror), pos(other.pos)
{
    key = 0;
    _type = this;
}

BehaviorMirror::~BehaviorMirror()
{
    // call the function to delete mirror whenever tyler makes it ioujdfsjklsdfjkl;dfsjkl;
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
    UNREFERENCED_PARAMETER(_input);
}

void BehaviorMirror::Init()
{
    if (Parent())
    {
        // Set collision handler for mirrors with lasers maybe??
        Collider* collider = Parent()->Has(Collider);
        if (Parent() && Parent()->Has(Transform)) 
        {
            //Parent()->GetImage()->position = currentPos;
            // set up mirror information
            ImageBuffer* image = Parent()->GetImage();
            if (image)
            {
                Transform* trans = Parent()->Has(Transform);
                gfxVector2 test = image->size;
                std::string file = Parent()->GetSpritePath();
                gfxVector2 up = { 0.0f, 1.0f };
                gfxVector2 right = { 1.0f, 0.0f };
                gfxVector2 left = { -1.0f, 0.0f };
                gfxVector2 down = { 0.0f, -1.0f };

                // horizontal
                mirror->xPos1 = { trans->GetTranslation()->x, trans->GetTranslation()->y + (test.y / 2) };
                mirror->xPos2 = { trans->GetTranslation()->x + (test.x), trans->GetTranslation()->y + (test.y / 2) };
                // vertical 
                mirror->yPos1 = { trans->GetTranslation()->x + (test.x / 2), trans->GetTranslation()->y };
                mirror->yPos2 = { trans->GetTranslation()->x + (test.y / 2), trans->GetTranslation()->y + test.y };
                //mirror->reflectDir = { 1 , 0 };
                // mirror->reflectFromLeft = true;
            }
        }
        Renderer::GetInstance()->laserHandler.AddMirror(mirror);
    }
}

Behavior* BehaviorMirror::Clone() const
{
    return new BehaviorMirror(*this);
}

void BehaviorMirror::Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
    if (Parent() && Parent()->Has(Transform)) {
        Vector2 position = *Parent()->Has(Transform)->GetTranslation();
        // set reflection position when the mirror stops moving or is not moving;
        //mirror->p

        // change to have dirty flag eventually
        ImageBuffer* image = Parent()->GetImage();
        Transform* trans = Parent()->Has(Transform);
        gfxVector2 test = image->size;
        float testvalue = (test.y / 1.5);
        std::string file = Parent()->GetSpritePath();
        // horizontal
        mirror->xPos1 = { trans->GetTranslation()->x, trans->GetTranslation()->y + (test.y / 2) };
        mirror->xPos2 = { trans->GetTranslation()->x + (test.x), trans->GetTranslation()->y + (test.y / 2) };
        // vertical 
        mirror->yPos1 = { trans->GetTranslation()->x + (test.x / 2), trans->GetTranslation()->y };
        mirror->yPos2 = { trans->GetTranslation()->x + (test.y / 2), trans->GetTranslation()->y + test.y };

    }
}

void BehaviorMirror::Read(json jsonData)
{

    Init();
    if (jsonData["Direction"].is_object())
    {
        json direction = jsonData["Direction"];
        //gfxVector2 angle;
        //angle.x = direction["DirectionX"];
        //angle.y = direction["DirectionY"];
        mirror->reflectDir.x = direction["DirectionX"];
        mirror->reflectDir.y = direction["DirectionY"];
    }


    for (auto& positions : jsonData["pos"])
    {
        // Extract "x" and "y" values, convert them to integers, and store in the vector
        Vector2 position{};
        position.x = (float)std::stoi(positions["x"].get<std::string>());
        position.y = (float)std::stoi(positions["y"].get<std::string>());
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
    UNREFERENCED_PARAMETER(thisone);
    UNREFERENCED_PARAMETER(other);
}

void BehaviorMirror::Controller(float dt)
{
    UNREFERENCED_PARAMETER(dt);
 //   float lerpFactor = 0.5f; // Adjust this value for speed

//    currentPos = currentPos + lerpFactor * (targetPos - currentPos);

 //   if (Parent() && Parent()->Has(Transform)) {
 //       Parent()->GetImage()->position = currentPos;
 //   }
}