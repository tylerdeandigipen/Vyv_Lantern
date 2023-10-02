#pragma once
#include "Behavior.h"
#include "Inputs.h"

class BehaviorPlayer : public Behavior
{
public:
	BehaviorPlayer();
	BehaviorPlayer(BehaviorPlayer const& other);
	~BehaviorPlayer();

	void SetInputHandler(Inputs* _input) override;

	void Init() override;

	Behavior* Clone() const override;

	void Update(float dt);
private:
	void Controller();
	Inputs* input;
};

