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
#include "EntityContainer.h"
#include "BehaviorReceiver.h"


BehaviorDoor::BehaviorDoor() : Behavior(Behavior::bDoor), mDestination(), AddedToForeGround(false), closedPPM(), openPPM(), tempImage(nullptr), isDoorClosed(true)
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

    UNREFERENCED_PARAMETER(dt);
    if (GetCurr() != GetNext())
    {
        SetCurr(GetNext());
    }
    
    if (GetCurr() == cIdle)
    {
        Entity* ent = EntityContainer::GetInstance()->FindByName(_receiver.c_str());
        if (ent && ent->GetComponent<BehaviorReceiver>())
        {
            if (EntityContainer::GetInstance()->FindByName(_receiver.c_str())->GetComponent<BehaviorReceiver>()->GetActive())
            {
                SetNext(cOpen);
            }
            else
            {
                if (isDoorClosed)
                {
                    SetNext(cIdle);
                }
                else
                {
                    SetNext(cClosed);
                }
            }
        }
    }
    else if (GetCurr() == cOpen)
    {
        if (isDoorClosed)
            Parent()->SetImage(openPPM);
        isDoorClosed = false;
        SetNext(cIdle);
    }
    else if (GetCurr() == cClosed)
    {
        if (!isDoorClosed)
            Parent()->SetImage(closedPPM);
        isDoorClosed = true;
        SetNext(cIdle);
    }
    
    
//#ifdef _DEBUG
//    Entity* ent = EntityContainer::GetInstance()->FindByName(_receiver.c_str());
//    if (ent && ent->GetComponent<BehaviorReceiver>() && GetDoorClosed() == true)
//    {
//        Inputs* input = Inputs::GetInstance();
//        if (input->keyPressed(SDL_SCANCODE_T))
//        {
//            EntityContainer::GetInstance()->FindByName(_receiver.c_str())->GetComponent<BehaviorReceiver>()->SetActive();
//        }
//    }
//#endif
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
    if (jsonData["PairName"].is_string())
    {
        _receiver = jsonData["PairName"];
    }
    if (Parent()->key.c_str())
    {
        _key = Parent()->key.c_str();
    }
    closedPPM = jsonData["ClosedSprite"];
    openPPM = jsonData["OpenSprite"];
}

void BehaviorDoor::DoorCollisionHandler(Entity* entity1, Entity* entity2)
{
    BehaviorDoor* door = nullptr;

    // Check if the win state is already set
    if (LevelBuilder::IsWinStateSet() == false && LevelBuilder::getDoor() == false) {
        // Check the player and the door collision
        if ((entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Door") == 0) ||
            (entity1->GetRealName().compare("Door") == 0 && entity2->GetRealName().compare("Player") == 0))
        {
            //LevelBuilder::SetWinState(true);

            if (entity1->GetRealName().compare("Door") == 0)
            {
                if (reinterpret_cast<BehaviorDoor*>(entity1->Has(Behavior)) && reinterpret_cast<BehaviorDoor*>(entity1->Has(Behavior))->GetName().compare("BehaviorDoor") == 0)
                {
                    door = reinterpret_cast<BehaviorDoor*>(entity1->Has(Behavior));
                }

            }
            else if (entity2->GetRealName().compare("Door") == 0)
            {
                if (reinterpret_cast<BehaviorDoor*>(entity2->Has(Behavior)) && reinterpret_cast<BehaviorDoor*>(entity2->Has(Behavior))->GetName().compare("BehaviorDoor") == 0)
                {
                    door = reinterpret_cast<BehaviorDoor*>(entity2->Has(Behavior));
                }
            }

#ifdef _DEBUG
            Inputs* input = Inputs::GetInstance();
            if (input->keyPressed(SDL_SCANCODE_E))
            {
                if (door->GetCurr() == cIdle)
                {
                    if (door->GetDoorClosed() == true)
                    {
                        door->isDoorClosed = false;
                        door->SetNext(cOpen);
                    }
                    else
                    {
                        door->isDoorClosed = true;
                        door->SetNext(cClosed);
                    }
                }
            }
#endif
            // Play sound effects only when the win state is set for the first time
            AudioManager.PlaySFX("cheer");
        }

    }
}

bool BehaviorDoor::GetDoorClosed()
{
    return isDoorClosed;
}

auto BehaviorDoor::GetDestinationPosition() -> Destination
{
    return mDestination;
}