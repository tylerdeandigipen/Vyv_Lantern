//------------------------------------------------------------------------------
//
// File Name:	BehaviorSwitch.h
// Author(s):	Louis Wang
// Purpose:		BehaviorSwitch class declaration for switch specific behavior
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Behavior.h"
#include "Vector.h"
#include <random>

class ImageBuffer;

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
	int GetMaxCount() { return maxCount; };
	bool GetLerped() { return isLerping; }
	void SetLerped() { isLerping = !isLerping; }

	// dont put in a bad index it wont like it very much
	gfxVector2 operator[](int i) { return pos[i]; };
	void Update(float dt);
	void Read(json jsonData) override;
	static void SwitchCollisionHandler(Entity* entity1, Entity* entity2);
	ImageBuffer* movePointDecal = nullptr;

private:
	void Controller(float dt);
	bool isLerping;
	int currentPos;
	int maxCount = 0;
	std::vector<gfxVector2> pos;
	static bool OnOff;
	int key = -12; // do not set doors arbitrary value to match thise plseas
};