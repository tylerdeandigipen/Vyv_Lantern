#pragma once
#include "Behavior.h"
#include "Inputs.h"
#include "Vector.h"
#include <random>

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

	Behavior* Clone() const override;

	void Update(float dt);
	void Read(json jsonData) override;
	static void SwitchOn(bool collided);
	static void MirrorCollisionHandler(Entity* thisone, Entity* other);

private:
	void Controller(float dt);
};