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
#include "SceneSystem.h"
#include "WinScene.h"
#include "LevelCreatorScene.h"
#include "Section1Final.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"

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
		{
			Parent()->SetImage(openPPM);
			AudioManager.PlaySFX("doorOpen");
		}

		isDoorClosed = false;
		SetNext(cIdle);
	}
	else if (GetCurr() == cClosed)
	{
		if (!isDoorClosed)
		{
			Parent()->SetImage(closedPPM);
			AudioManager.PlaySFX("doorClosed");
		}

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
	if (jsonData["NextScene"].is_string())
	{
		_nextScene = jsonData["NextScene"];
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

		if (door != nullptr && door->isDoorClosed == false)
		{
			if (door->_nextScene == "Section1Final")
				SceneSystem::GetInstance()->SetScene(Section1FinalGetInstance());
			else if (door->_nextScene == "Level1")
				SceneSystem::GetInstance()->SetScene(Level1GetInstance());
			else if (door->_nextScene == "Level2")
				SceneSystem::GetInstance()->SetScene(Level2GetInstance());
			else if (door->_nextScene == "Level3")
				SceneSystem::GetInstance()->SetScene(Level3GetInstance());
			else if (door->_nextScene == "Level4")
				SceneSystem::GetInstance()->SetScene(Level4GetInstance());
			else if (door->_nextScene == "WinScene")
			{
				if (LevelBuilder::GetWinState())
				{
					AudioManager.PlayMusic("cheer");
					SceneSystem::GetInstance()->SetScene(WinSceneGetInstance());
				}
			}
		}

#ifdef _DEBUG
		Inputs* input = Inputs::GetInstance();
		if (input->keyPressed(SDL_SCANCODE_7))
		{
			SceneSystem::GetInstance()->SetScene(Section1FinalGetInstance());
			if (door->GetCurr() == cIdle)
			{
				if (door->GetDoorClosed() == true)
				{
					door->SetNext(cOpen);
				}
				else
				{
					// LevelBuilder::SetWinState(true);
					if (LevelBuilder::IsWinStateSet() == false)
					{
						if (door->_nextScene == "WinScene")
						{
							AudioManager.PlaySFX("cheer");
							SceneSystem::GetInstance()->SetScene(WinSceneGetInstance());
						}
						else if (door->_nextScene == "LevelCreatorScene")
							SceneSystem::GetInstance()->SetScene(LevelCreatorSceneGetInstance());
						else if (door->_nextScene == "Section1Final")
							SceneSystem::GetInstance()->SetScene(Section1FinalGetInstance());
					}
					door->SetNext(cClosed);
				}
			}
		}
#endif
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