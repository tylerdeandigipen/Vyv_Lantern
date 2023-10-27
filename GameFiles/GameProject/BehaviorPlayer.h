//------------------------------------------------------------------------------
//
// File Name:	BehaviorPlayer.h
// Author(s):	Michael Howard (michael.howard)
// Purpose:		The players behavior
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Behavior.h"
#include "Inputs.h"
#include "Vector.h"
#include <random>

class BehaviorPlayer : public Behavior
{
public:
	BehaviorPlayer();
	BehaviorPlayer(BehaviorPlayer const& other);
	~BehaviorPlayer();


	std::string GetName() override;
	static std::string Name();

	void SetInputHandler(Inputs* _input) override;

	void Init() override;

	Behavior* Clone() const override;

	void Update(float dt);
	void Read(json jsonData) override;
	static void PlayerCollisionHandler(Entity* thisone, Entity* other);

private:
	float timeBetweenBlink = 8;
	float timeDuringBlink = 0.115f;
	float timer = 0;
	bool isBlinking = false;
	float MIN_RAND = 6.0f, MAX_RAND = 9.0f;
	void Controller(float dt);
	Inputs* input;
	float playerMoveSpeed;
	int wehavewalls[19] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 45, 46, 48, 29, 26, 31, 41, 39 };

	bool checkWalls(gfxVector2 position);
};

