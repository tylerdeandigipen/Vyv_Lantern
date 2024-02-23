//------------------------------------------------------------------------------
//
// File Name:	BehaviorDoor.cpp
// Author(s):	Michael Howard (michael.howard)
// Purpose:		BehaviorDoor class implementation for door specific behavior
// 
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "AudioEngine.h"
#include "BehaviorDoor.h"
#include "Entity.h"
#include "Collider.h"
#include "Inputs.h"
#include "ImageBuffer.h"
#include "LevelBuilder.h"
#include "Physics.h"
#include "Renderer.h"
#include "Transform.h"
#include "Renderer.h"
#include "TbdTestScene.h"

BehaviorDoor::BehaviorDoor() : Behavior(Behavior::Door), mDestination(), AddedToForeGround(false), closedPPM(), openPPM(), tempImage(nullptr)
{
    _type = this;
}

BehaviorDoor::BehaviorDoor(BehaviorDoor const& other) : Behavior(other), isDoorClosed(other.isDoorClosed), AddedToForeGround(false), closedPPM(other.closedPPM), openPPM(other.openPPM), tempImage(other.tempImage)
{
    _type = this;
}

BehaviorDoor::~BehaviorDoor()
{
	/*if (tempImage != nullptr)
	{
		delete tempImage;
		tempImage = nullptr;
	}*/
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
   // if (!AddedToForeGround)
   // {
   //     Renderer::GetInstance()->foregroundLayer->AddSprite(Parent()->GetImage());
   //     AddedToForeGround = true;
   // }
    if ((LevelBuilder::getDoor() == true) && (isDoorClosed == true))
    {
        SetNext(1);
        isDoorClosed = false;
    }
    UNREFERENCED_PARAMETER(dt);
    if (GetCurr() != GetNext())
    {
        SetCurr(GetNext());
    }
    if (GetCurr() == 1)
    {   
        Parent()->SetImage(openPPM);
        SetNext(cIdle);
    }
}


void BehaviorDoor::Read(json jsonData)
{
    Init();
    if (jsonData["DoorClosed"].is_boolean())
    {
        isDoorClosed = jsonData["DoorClosed"];
        if (isDoorClosed == true)
        {
            SetCurr(cClosed);
        }
        else
        {
            SetCurr(cOpen);
        }
    }
    if (jsonData["Destination"].is_object())
    {
		mDestination.position.x = jsonData["Destination"]["x"];
		mDestination.position.y = jsonData["Destination"]["y"];
	}
    closedPPM = jsonData["ClosedSprite"];
    openPPM = jsonData["OpenSprite"];
}

void BehaviorDoor::DoorCollisionHandler(Entity* entity1, Entity* entity2)
{
    BehaviorDoor* door = nullptr;

    if (reinterpret_cast<BehaviorDoor*>(entity2->Has(Behavior)) && reinterpret_cast<BehaviorDoor*>(entity2->Has(Behavior))->GetName().compare("BehaviorDoor") == 0)
    {
        door = reinterpret_cast<BehaviorDoor*>(entity2->Has(Behavior));
    }
    else if (reinterpret_cast<BehaviorDoor*>(entity1->Has(Behavior)) && reinterpret_cast<BehaviorDoor*>(entity1->Has(Behavior))->GetName().compare("BehaviorDoor") == 0)
    {
        door = reinterpret_cast<BehaviorDoor*>(entity1->Has(Behavior));
    }

    // Check if the win state is already set
    if (LevelBuilder::IsWinStateSet() == false && LevelBuilder::getDoor() == false) {
        // Check the player and the door collision
        if ((entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Door") == 0) ||
            (entity1->GetRealName().compare("Door") == 0 && entity2->GetRealName().compare("Player") == 0))
        {
            //LevelBuilder::SetWinState(true);

            if (entity1->GetRealName().compare("Door") == 0)
            {
#ifdef _DEBUG
                Inputs* input = Inputs::GetInstance();
                if (input->keyPressed(SDL_SCANCODE_E))
                {
                    entity1->SetImage(door->openPPM);
                    door->isDoorClosed = false;
                    door->SetCurr(cOpen);
                }
#endif
            }
            else if (entity2->GetRealName().compare("Door") == 0)
            {
#ifdef _DEBUG
                Inputs* input = Inputs::GetInstance();
                if (input->keyPressed(SDL_SCANCODE_E))
                {
                    entity2->SetImage(door->openPPM);
                    door->isDoorClosed = false;
                    door->SetCurr(cOpen);
                }
#endif
            }

            // Play sound effects only when the win state is set for the first time
            AudioManager.PlaySFX("cheer");
        }

    }
}

bool BehaviorDoor::GetDoorClosed()
{
    return isDoorClosed;
}

void BehaviorDoor::SetDoorClosed()
{
    if (isDoorClosed == false)
	{
		isDoorClosed = true;
	}
}

auto BehaviorDoor::GetDestinationPosition() -> Destination
{
    return mDestination;
}