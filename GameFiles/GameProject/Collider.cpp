//------------------------------------------------------------------------------
//
// File Name:	Collider.cpp
// Author(s):	Michael Howard (michael.howard)
// Purpose:		collider component
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Collider.h"
#include "Entity.h"

Collider::Collider(Collider::ColliderType _type) : Component(Component::cCollider), bType(_type)
{
}

Collider::Collider(Collider const& other) : Component(other), bType(other.bType)
{
}

Collider::~Collider()
{
}

Component* Collider::Clone() const
{
	return new Collider(*this);
}

void Collider::Read(json jsonData)
{
	jsonData = jsonData;
}

void Collider::Update(float dt)
{
	dt = dt;
}

void Collider::SetCollisionHandler(std::function<void(Entity*, Entity*)> _handler)
{
	handler = _handler;
}

std::function<void(Entity*, Entity*)> Collider::GetCollisionHandler()
{
	return handler;
}


