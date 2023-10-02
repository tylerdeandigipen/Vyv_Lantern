#pragma once

#include "Component.h"
#include "Inputs.h"

class Behavior : public Component
{
public:
	enum BehaviorType { Player };
	_inline Behavior::BehaviorType type() const { return bType; };
	_inline Behavior* Type() const { return _type; };

	Behavior(BehaviorType _type);
	Behavior(Behavior const& other);

	virtual ~Behavior();
	void Read(Stream stream) override;

	virtual void SetInputHandler(Inputs* input) {};

	Component* Clone(void) const override;

	virtual void Init() {} ;
	void Update(float dt);
	Behavior* _type;

private:
	BehaviorType bType;


};

