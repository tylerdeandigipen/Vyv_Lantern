//------------------------------------------------------------------------------
//
// File Name:	Physics.cpp
// Author(s):	Michael Howard
// Purpose:		Class for all Entity Physics
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Physics.h"
#include "Entity.h"
#include "Transform.h"

Physics::Physics() : Component(Component::cPhysics)
{
}

Physics::Physics(Physics const& phys) : Component(phys)
{
}

Physics::~Physics()
{
}

Component* Physics::Clone() const
{
	return new Physics(*this);
}

std::string Physics::Name()
{
	return "Physics";
}

const gfxVector2 Physics::GetOldTranslation()
{
	return oldTranslation;
}

std::string Physics::GetName()
{
	return std::string();
}

void Physics::Read(json jsonData)
{
	if (jsonData["OldTranslation"].is_object())
	{
		json old = jsonData["OldTranslation"];
		oldTranslation.x = old["x"];
		oldTranslation.y = old["y"];
	}
}

void Physics::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	Transform* transform = Parent()->Has(Transform);
	if (transform)
	{
		float _rotation = transform->GetRotation();
		const gfxVector2* _translation = transform->GetTranslation();
		oldTranslation = *_translation;
	}
}