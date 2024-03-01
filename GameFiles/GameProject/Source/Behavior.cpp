//------------------------------------------------------------------------------
//
// File Name:	Behavior.cpp
// Author(s):	Michael Howard
// Purpose:		Defines entity behavior.
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Behavior.h"

Behavior::Behavior(Behavior::BehaviorType type) : Component(Component::cBehavior), bType(type), _type(), nextState(0), currState(0)
{
}

Behavior::Behavior(Behavior const& other) : Component(other), bType(other.bType), _type(other._type), nextState(other.nextState), currState(other.currState)
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

void Behavior::SetCurr(int num)
{
	currState = num;
}

void Behavior::SetNext(int num)
{
	nextState = num;
}

int Behavior::GetCurr() const
{
	return currState;
}

int Behavior::GetNext() const
{
	return nextState;
}
