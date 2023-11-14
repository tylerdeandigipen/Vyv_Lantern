#pragma once
#include "Behavior.h"
#include "Inputs.h"
#include "Vector.h"
#include <random>

class BehaviorSwitch : public Behavior
{
public:
	BehaviorSwitch();
	BehaviorSwitch(BehaviorSwitch const& other);
	~BehaviorSwitch();

	std::string GetName() override;
	static std::string Name();

	void SetInputHandler(Inputs* _input) override;

	void Init() override;

	Behavior* Clone() const override;

	void Update(float dt);
	void Read(json jsonData) override;
	static void SwitchCollisionHandler(Entity* thisone, Entity* other);

private:
	void Controller(float dt);
	bool OnOff = false;
	static int count;
	static int maxCount;
};