//------------------------------------------------------------------------------
//
// File Name:	BehaviorEmitter.h
// Author(s):	Michael Howard
// Purpose:		Declaration of BehaviorEmitter class for controlling emitters
// 
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Behavior.h"
//#include "LaserSystem.h"
#include "TBDLasers.h"

class gfxVector2;

class BehaviorEmitter : public Behavior
{
public:
	BehaviorEmitter();
	BehaviorEmitter(BehaviorEmitter const& other);
	~BehaviorEmitter();

	std::string GetName() override;
	static std::string Name();

	void SetInputHandler(Inputs* _input) override;

	void Init() override;

	Behavior* Clone() const override;
	void Update(float dt);
	void Read(json jsonData) override;
	static void SwitchOn(bool collided);
	static void EmitterCollisionHandler(Entity* thisone, Entity* other); // may not need one

private:
	//reflector* reflect;
	Laser* mirror;
	std::vector<gfxVector2> pos;
	static int count;
	int key;
	static int maxCount;
	static gfxVector2 currentPos; // Current position of the mirror
	static gfxVector2 targetPos; // Target position for interpolation
	bool inited = false;
};