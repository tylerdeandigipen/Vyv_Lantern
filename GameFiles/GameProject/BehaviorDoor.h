//------------------------------------------------------------------------------
//
// File Name:	BehaviorDoor.h
// Author(s):	Michael Howard (michael.howard)
// Purpose:		BehaviorDoor class declaration for door specific behavior
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Behavior.h"

class ImageBuffer;

class BehaviorDoor : public Behavior
{
public:
	BehaviorDoor();
	BehaviorDoor(BehaviorDoor const& other);
	~BehaviorDoor();
	enum { cClosed, cOpen, cIdle };

	std::string GetName() override;
	static std::string Name();

	void Init() override;

	Behavior* Clone() const override;

	void Update(float dt);
	void Read(json jsonData) override;
	static void DoorCollisionHandler(Entity* thisone, Entity* other);
	bool GetDoorClosed();
	void SetDoorClosed();

private:
	bool isDoorClosed;
	bool AddedToForeGround;
	std::string closedPPM;
	std::string openPPM;
	ImageBuffer* tempImage;
};

