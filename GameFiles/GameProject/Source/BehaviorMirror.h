//------------------------------------------------------------------------------
//
// File Name:	BehaviorMirror.h
// Author(s):	Louis Wang
// Purpose:		Declaration of BehaviorMirror class for controlling mirror 
//              entities.
// 
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Behavior.h"
#include "LaserSystem.h"

class gfxVector2;
struct reflector;

class BehaviorMirror : public Behavior
{
public:
	BehaviorMirror();
	BehaviorMirror(BehaviorMirror const& other);
	~BehaviorMirror();

	std::string GetName() override;
	static std::string Name();

	void SetInputHandler(Inputs* _input) override;

	void Init() override;
	int GetKey() { return key; }

	Behavior* Clone() const override;
	void Update(float dt);
	void Read(json jsonData) override;
	static void SwitchOn(bool collided);
	static void MirrorCollisionHandler(Entity* thisone, Entity* other);

private:
	void Controller(float dt);
	reflector* reflect;
	std::vector<gfxVector2> pos;
	static int count;
	int key;
	static int maxCount;
	static gfxVector2 currentPos; // Current position of the mirror
	static gfxVector2 targetPos; // Target position for interpolation
};