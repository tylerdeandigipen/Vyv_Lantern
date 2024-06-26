//------------------------------------------------------------------------------
//
// File Name:	BehaviorSwitch.cpp
// Author(s):	Louis Wang
// Purpose:		Implementation of behaviormirror class for controlling switch
//              entities.
//
// Copyright ?2023 DigiPen (USA) Corporation.
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
#include <stdlib.h>

bool BehaviorSwitch::OnOff = false;

BehaviorSwitch::BehaviorSwitch() : Behavior(Behavior::bSwitch), isLerping(false), currentPos(0), maxCount(0)
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
	Renderer::GetInstance()->FreeDecalFromRenderer(movePointDecal);
	delete movePointDecal;
	movePointDecal = NULL;
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
	UNREFERENCED_PARAMETER(_input);
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
		gfxVector2 lerped = lerpValue(*Parent()->Has(Transform)->GetTranslation(), pos[currentPos], 1.5f * dt);
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
	std::string tempSprite = "./Assets/PPM/MovePointDecal.ppm";
	movePointDecal = new ImageBuffer(tempSprite);
	/*What values to load into the switches here*/
	maxCount = jsonData["NumPositions"] - 1;
	for (auto& positions : jsonData["pos"])
	{
		// Extract "x" and "y" values, convert them to integers, and store in the vector
		float x = (float)std::stoi(positions["x"].get<std::string>());
		float y = (float)std::stoi(positions["y"].get<std::string>());
		Renderer::GetInstance()->AddDecal(movePointDecal, Vector2{ x, y } - Vector2{ 2,2 } + Vector2{ 4,4 });// 2, 2 is half size of movepoint and 4,4 is half of mirror sprite
		pos.push_back({ x,y });
	}
	key = jsonData["key"];

	for (int i = 0; i < pos.size() - 1; i++)
	{
		Renderer::GetInstance()->DrawLine(pos[i] + Vector2{ 4,4 }, pos[i + 1] + Vector2{ 4,4 }, Color{ 186,185,182,255 }, Renderer::GetInstance()->backgroundLayer, 0.75f);
		Renderer::GetInstance()->DrawLine(pos[i] + Vector2{ 3,3 }, pos[i + 1] + Vector2{ 3,3 }, Color{ 186,185,182,255 }, Renderer::GetInstance()->backgroundLayer, 0.75f);
	}
}

ImageBuffer* prompt = NULL;
static bool isColliding = false;
void BehaviorSwitch::SwitchCollisionHandler(Entity* entity1, Entity* entity2)
{
	Inputs* input = Inputs::GetInstance();
	if (prompt != NULL)
		prompt->isCulled = true;

	// find which switch is actually a switch
	BehaviorSwitch* switch1 = entity1->GetComponent<BehaviorSwitch>();

	//BehaviorSwitch* switch1 = reinterpret_cast<BehaviorSwitch*>(entity1->Has(Behavior));
	BehaviorSwitch* switch2 = entity2->GetComponent<BehaviorSwitch>();
	switch1->GetLerped();

	// BehaviorSwitch* switch2 = reinterpret_cast<BehaviorSwitch*>(entity2->Has(Behavior));
	BehaviorSwitch* theSwitch = NULL;
	if (switch2 && switch2->GetName().compare("BehaviorSwitch") == 0)
	{
		theSwitch = switch2;
	}
	else if (switch1 && switch1->GetName().compare("BehaviorSwitch") == 0)
	{
		theSwitch = switch1;
	}
	if (entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Switch") == 0 || entity1->GetRealName().compare("Switch") == 0 && entity2->GetRealName().compare("Player") == 0)
	{
		/*Check if player is inside switch*/
		isColliding = true;

		if (prompt != NULL)
		{
			prompt->position.x = entity2->GetImage()->position.x + 10;
			prompt->position.y = entity2->GetImage()->position.y - 10;
		}

		if (prompt == NULL)
		{
			prompt = new ImageBuffer{ "./Assets/PPM/Press_E.ppm" };
			Renderer::GetInstance()->AddObject(prompt);
		}
		if (theSwitch && (theSwitch->GetLerped() == false))
		{
			prompt->isCulled = false;

			if (input->keyPressed(SDL_SCANCODE_E))
			{
				// swaps bool value
				theSwitch->SetLerped();
				OnOff = true;
				/*Mirror will move here*/

				int audioPlay = rand() % 5 + 1;

				//BehaviorMirror::SwitchOn(OnOff);
				if (audioPlay == 1)
				{
					AudioManager.PlaySFX("mirrorMove1", 0.2f);
				}
				else if (audioPlay == 2)
				{
					AudioManager.PlaySFX("mirrorMove2", 0.2f);
				}
				else if (audioPlay == 3)
				{
					AudioManager.PlaySFX("mirrorMove3", 0.2f);
				}
				else if (audioPlay == 4)
				{
					AudioManager.PlaySFX("mirrorMove4", 0.2f);
				}
				else if (audioPlay == 5)
				{
					AudioManager.PlaySFX("mirrorMove5", 0.2f);
				}

				// temporary win condition /
				if (theSwitch->currentPos == theSwitch->maxCount)
				{
				//	AudioManager.PlaySFX("doorOpen", 1.0); // maybe make the sound quieter the further the player is
				//	LevelBuilder::setDoor(true);
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
			if (prompt)
			{
				prompt->isCulled = true;
			}
		}
	}
}

void BehaviorSwitch::Controller(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// DO SWITCH THINGS HERE
}