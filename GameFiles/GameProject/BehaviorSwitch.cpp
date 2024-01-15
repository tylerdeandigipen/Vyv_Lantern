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

bool BehaviorSwitch::OnOff = false;

BehaviorSwitch::BehaviorSwitch() : Behavior(Behavior::Switch), isLerping(false), currentPos(0), maxCount(0)
{
	_type = this;
}

BehaviorSwitch::BehaviorSwitch(BehaviorSwitch const& other) : Behavior(other), isLerping(other.isLerping), currentPos(other.currentPos), maxCount(other.maxCount)
{
	_type = this;
}

BehaviorSwitch::~BehaviorSwitch()
{
    currentPos = 0;
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

gfxVector2 lerpValue(gfxVector2 a, gfxVector2 b, float t)
{
    gfxVector2 lerpedVector = a;

    lerpedVector.x += t * (b.x - a.x);
    lerpedVector.y += t * (b.y - a.y);

    return lerpedVector;
}

void BehaviorSwitch::Update(float dt)
{
    if (Parent())
        Controller(dt);
    if (GetLerped() == true)
    {
        gfxVector2 lerped = lerpValue(*Parent()->Has(Transform)->GetTranslation(), pos[currentPos], 0.04f);
        Parent()->Has(Transform)->SetTranslation(lerped);
        if (lerped.x >= (pos[currentPos].x - 1) &&
            lerped.x <= (pos[currentPos].x + 1) &&
            lerped.y >= (pos[currentPos].y - 1) &&
            lerped.y <= (pos[currentPos].y + 1))
        {
            Parent()->Has(Transform)->SetTranslation(pos[currentPos]);
            SetLerped();
        }
    }
}

void BehaviorSwitch::Read(json jsonData)
{
    Init();
    /*What values to load into the switches here*/
    maxCount = jsonData["NumPositions"] - 1;
    for (auto& positions : jsonData["pos"])
    {
        // Extract "x" and "y" values, convert them to integers, and store in the vector
        float x = std::stoi(positions["x"].get<std::string>());
        float y = std::stoi(positions["y"].get<std::string>());
        
        pos.push_back({ x,y });
    }
    key = jsonData["key"];
}



ImageBuffer* prompt = NULL;
static bool isColliding = false;
void BehaviorSwitch::SwitchCollisionHandler(Entity* entity1, Entity* entity2)
{
    Inputs* input = Inputs::GetInstance();
    if (prompt != NULL)
        prompt->isCulled = true;

    // find which switch is actually a switch 
    BehaviorSwitch* switch1 = reinterpret_cast<BehaviorSwitch*>(entity1->Has(Behavior));
    BehaviorSwitch* switch2 = reinterpret_cast<BehaviorSwitch*>(entity2->Has(Behavior));
    BehaviorSwitch* theSwitch = NULL;
    if (switch2->GetName().compare("BehaviorSwitch") == 0)
    {
        theSwitch = switch2;
    }
    else if (switch1->GetName().compare("BehaviorSwitch") == 0)
    {
        theSwitch = switch1;
    }

    if (entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Switch") == 0 || entity1->GetRealName().compare("Switch") == 0 && entity2->GetRealName().compare("Player") == 0)
    {
        /*Check if player is inside switch*/
        isColliding = true;
        if (prompt == NULL)
        {
            prompt = new ImageBuffer{"./Assets/PPM/Press_E.ppm"};
            Renderer::GetInstance()->AddObject(prompt);
        }
        if (isColliding && theSwitch && (theSwitch->GetLerped() == false))
        {
            prompt->isCulled = false;

            if (input->keyPressed(SDL_SCANCODE_E))
            {
                // swaps bool value
                theSwitch->SetLerped();
                OnOff = true;
                /*Mirror will move here*/
                //BehaviorMirror::SwitchOn(OnOff);
                AudioManager.PlaySFX("laser");

                // temporary win condition
                if (theSwitch->currentPos == theSwitch->maxCount)
                {
                    AudioManager.PlaySFX("door");
                    LevelBuilder::setDoor(true);
                    theSwitch->currentPos = 0;
                }
                else
                {
                    theSwitch->currentPos++;
                }
                //prompt->isCulled = true;
                if (prompt->isCulled == false)
                {
                    prompt->isCulled = true;
                }
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
