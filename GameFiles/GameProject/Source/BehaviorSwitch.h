//------------------------------------------------------------------------------
//
// File Name:	BehaviorSwitch.h
// Author(s):	Louis Wang
// Purpose:		BehaviorSwitch class declaration for switch specific behavior
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Behavior.h"

#include <random>

class Inputs;
class gfxVector2;

class BehaviorSwitch : public Behavior
{
public:
	BehaviorSwitch();
	BehaviorSwitch(BehaviorSwitch const& other);
	~BehaviorSwitch();

	std::string GetName();
	static std::string Name();

	void SetInputHandler(Inputs* _input) override;

	void Init() override;

	Behavior* Clone() const override;

	int GetKey() { return key; }
	bool GetLerped() { return isLerping; }
	void SetLerped() { isLerping = !isLerping; }


	void Update(float dt);
	void Read(json jsonData) override;
	static void SwitchCollisionHandler(Entity* entity1, Entity* entity2);

private:
	void Controller(float dt);
	bool isLerping;
	int currentPos;
	int maxCount;
	std::vector<gfxVector2> pos;
	static bool OnOff;
	int key;
};