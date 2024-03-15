#pragma once
#include "Behavior.h"
#include "TBDLasers.h"

class gfxVector2;
class BehaviorReceiver : public Behavior
{
public:
	BehaviorReceiver();
	BehaviorReceiver(BehaviorReceiver const& other);
	~BehaviorReceiver();

	std::string GetName() override;
	static std::string Name();

	void SetInputHandler(Inputs* _input) override;

	void Init() override;

	Behavior* Clone() const override;
	void Update(float dt);
	void Read(json jsonData) override;
	static void EmitterCollisionHandler(Entity* thisone, Entity* other); // may not need one
	bool GetActive() { return receiver->isActivated; }
	void SetActive()
	{
		receiver->isActivated ? receiver->isActivated = false : receiver->isActivated = true;
	}

private:
	//bool inited = false;
	CheckPoint* receiver;
};

