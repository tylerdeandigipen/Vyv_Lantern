//------------------------------------------------------------------------------
//
// File Name:	BehaviorSwitch.cpp
// Author(s):	Louis Wang
// Purpose:		Implementation of behaviormirror class for controlling switch 
//              entities.
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "BehaviorSwitch.h"
#include "BehaviorMirror.h"
#include "Collider.h"
#include "Entity.h"
#include "Inputs.h"
#include "Transform.h"
#include "Physics.h"
#include "LevelBuilder.h"
#include "Maths/Vector.h"
#include "AudioEngine.h"
#include "Collision.h"
#include "FontSystem.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include "Vector.h"

int BehaviorSwitch::count = 0;
int BehaviorSwitch::maxCount = 4;
std::vector<gfxVector2> BehaviorSwitch::pos;
bool BehaviorSwitch::OnOff = false;
int BehaviorSwitch::key = 0;


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
    count = 0;
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
    for (auto& positions : jsonData["pos"])
    {
        // Extract "x" and "y" values, convert them to integers, and store in the vector
        float x = std::stoi(positions["x"].get<std::string>());
        float y = std::stoi(positions["y"].get<std::string>());
        
        pos.push_back({ x,y });
    }
    key = jsonData["key"];
}

gfxVector2 lerpValue(gfxVector2 a, gfxVector2 b, float t)
{
    gfxVector2 lerpedVector;

    lerpedVector.x = a.x + t * (b.x - a.x);
    lerpedVector.y = a.y + t * (b.y - a.y);

    return lerpedVector;
}


ImageBuffer* prompt = NULL;
static bool isColliding = false;
void BehaviorSwitch::SwitchCollisionHandler(Entity* entity1, Entity* entity2)
{
    Inputs* input = Inputs::GetInstance();
    if (prompt != NULL)
        prompt->isCulled = true;

    if (entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Switch") == 0 || entity1->GetRealName().compare("Switch") == 0 && entity2->GetRealName().compare("Player") == 0)
    {
        /*Check if player is inside switch*/
        isColliding = true;
        if (prompt == NULL)
        {
            prompt = new ImageBuffer{"./Assets/PPM/Press_E.ppm"};
            Renderer::GetInstance()->AddObject(prompt);
        }
        if (isColliding)
        {
            prompt->isCulled = false;
            if (entity2->GetRealName().compare("Switch") == 0)
            {
                prompt->position.x = entity2->GetImage()->position.x + 10;
                prompt->position.y = entity2->GetImage()->position.y - 10;
                
            }
            if (entity1->GetRealName().compare("Switch") == 0)
            {
                prompt->position.x = entity1->GetImage()->position.x + 10;
                prompt->position.y = entity2->GetImage()->position.y - 10;
            }

            if (input->keyPressed(SDL_SCANCODE_E))
            {
                OnOff = true;
                /*Mirror will move here*/
                BehaviorMirror::SwitchOn(OnOff);
                AudioManager.PlaySFX("laser");
                if (count < maxCount)
                {
                    float progress = 0.0f; // Progress value for lerping
                    if (entity2->GetRealName().compare("Switch") == 0)
                    {
                        //gfxVector2 lerped = lerpValue(entity2->GetImage()->position, pos[count], progress);
                        entity2->GetImage()->position = pos[count];
                    }
                    else if (entity1->GetRealName().compare("Switch") == 0)
                    {
                        //gfxVector2 lerped = lerpValue(entity1->GetImage()->position, pos[count], progress);
                        entity1->GetImage()->position = pos[count];
                    }
                    progress += 0.001f;
                    count++;
                    if (count == 4)
                    {
                        AudioManager.PlaySFX("door");
                        LevelBuilder::setDoor(true);
                    }
                    //prompt->isCulled = true;
                    if (prompt->isCulled == false)
                    {
                        prompt->isCulled = true;
                    }
                }
                /*
                else
                {
                    
                    count = 0;
                    if (entity2->GetRealName().compare("Switch") == 0)
                    {
                        entity2->GetImage()->position = pos[count];
                    }
                    else if (entity1->GetRealName().compare("Switch") == 0)
                    {
                        entity1->GetImage()->position = pos[count];
                    }
                    

                }
                */
                OnOff = false;
                /*Move switch*/
            }
        }
        else
        {
            isColliding = false;
            if (prompt)
                prompt->isCulled = false;
        }
    }
}



void BehaviorSwitch::Controller(float dt)
{
    // DO SWITCH THINGS HERE
}
