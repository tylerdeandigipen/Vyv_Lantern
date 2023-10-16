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

std::string Physics::GetName()
{
	return std::string();
}

void Physics::Read(json jsonData)
{
	// for when we get json
	// 
}

void Physics::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	/*Transform* transform = Parent()->Has(Transform);
	if (transform)
	{
		float _rotation = transform->GetRotation();
		const Vector2D* _translation = transform->GetTranslation();
		Vector2D temp = { 0, 0 };

		oldTranslation = *_translation;
		_rotation += rotationalVelocity * dt;
		Vector2DScaleAdd(&velocity, &acceleration, &velocity, dt);
		Vector2DScaleAdd(&temp, &velocity, _translation, dt);
		transform->SetTranslation(&temp);
		transform->SetRotation(_rotation);
	}*/
}
