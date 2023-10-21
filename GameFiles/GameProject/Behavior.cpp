#include "Behavior.h"

Behavior::Behavior(Behavior::BehaviorType type) : Component(Component::cBehavior), bType(type), _type()
{
}

Behavior::Behavior(Behavior const& other) : Component(other), bType(other.bType), _type(other._type)
{
}

Behavior::~Behavior()
{
}

Component* Behavior::Clone() const
{
	return new Behavior(*this);
}

void Behavior::Read(json jsonData)
{
	jsonData = jsonData;
}

void Behavior::Update(float dt)
{
	dt = dt;
}