//------------------------------------------------------------------------------
//
// File Name:	ColliderAABB.cpp
// Author(s):	Michael, Louis
// Purpose:		AABB collider for most if not all game objects
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "ColliderAABB.h"

ColliderAABB::ColliderAABB() : Collider(ColliderType::AABB)
{

}

ColliderAABB::ColliderAABB(ColliderAABB const& other) : Collider(other)
{

}

ColliderAABB::~ColliderAABB()
{

}
//virtual CreateComponent(mType);

// used to invoke the copy constructor
Collider* ColliderAABB::Clone(void) const
{
	return new ColliderAABB(*this);
}

void ColliderAABB::Check(Collider* other)
{
	if (IsColliding(*other))
	{
		if (GetCollisionHandler())
			GetCollisionHandler()(Parent(), other->Parent());
		if (other->GetCollisionHandler())
			other->GetCollisionHandler()(other->Parent(), Parent());
	}
}

bool ColliderAABB::IsColliding(const Collider& other)
{
	//LOUISSS you can do your checking here

	return false;
}

std::string ColliderAABB::Name()
{
	return "ColliderAABB";
}

void ColliderAABB::Update(float dt)
{
	dt = dt;
}

void ColliderAABB::Read(json jsonData)
{
	UNREFERENCED_PARAMETER(jsonData);
}
