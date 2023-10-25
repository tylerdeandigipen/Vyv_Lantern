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
	void Controller(float dt);
	Inputs* input;
	float playerMoveSpeed;
};

