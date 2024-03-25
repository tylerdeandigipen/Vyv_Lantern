//------------------------------------------------------------------------------
//
// File Name:	BehaviorDoor.h
// Author(s):	Michael Howard (michael.howard)
// Purpose:		BehaviorDoor class declaration for door specific behavior
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Behavior.h"
#include "Vector.h"
#include "TBDLasers.h"
#include <utility>

class ImageBuffer;

struct Destination
{
	std::string name;
	Vector2 position;
};

class BehaviorDoor : public Behavior
{
public:
	BehaviorDoor();
	BehaviorDoor(BehaviorDoor const& other);
	~BehaviorDoor();
	enum { cIdle, cOpen, cClosed };

	std::string GetName() override;
	static std::string Name();

	void Init() override;

	Behavior* Clone() const override;

	void Update(float dt);
	void Read(json jsonData) override;
	static void DoorCollisionHandler(Entity* entity1, Entity* entity2);
	bool GetDoorClosed();
	auto GetDestinationPosition() -> Destination;
	auto GetPairName() -> std::string { return _receiver; };

private:
	Destination mDestination;
	bool isDoorClosed = true;
	bool AddedToForeGround;
	std::string closedPPM;
	std::string openPPM;
	ImageBuffer* tempImage;
	std::string _receiver;
	std::string _nextScene;
	std::string _key;
};

