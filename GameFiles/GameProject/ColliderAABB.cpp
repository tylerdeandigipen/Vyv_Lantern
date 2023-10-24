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
#include "Entity.h"
#include "ImageBuffer.h"
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
	ImageBuffer checks = *Parent()->GetImage();
	ImageBuffer otherChecks = *other.Parent()->GetImage();

	checks.aabb.min = { checks.position.x, checks.position.y};
	checks.aabb.max = { checks.position.x + checks.BufferSizeX, 
		checks.position.y + checks.BufferSizeY };

	otherChecks.aabb.min = { otherChecks.position.x, otherChecks.position.y };
	otherChecks.aabb.max = { otherChecks.position.x + otherChecks.BufferSizeX,
		otherChecks.position.y + otherChecks.BufferSizeY };

	return CollisionCheck(checks.aabb, otherChecks.aabb);
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
