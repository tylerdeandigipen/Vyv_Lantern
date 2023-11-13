#pragma once
#include "Behavior.h"
class BehaviorDoor : public Behavior
{
public:
	BehaviorDoor();
	BehaviorDoor(BehaviorDoor const& other);
	~BehaviorDoor();


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
};

