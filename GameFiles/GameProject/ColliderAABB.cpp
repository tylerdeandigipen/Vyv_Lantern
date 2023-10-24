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
#include "Transform.h"
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
	
	Vector2 firstPosition = *Parent()->Has(Transform)->GetTranslation();
	Vector2 secondPosition = *other.Parent()->Has(Transform)->GetTranslation();


	checks.aabb.min = { firstPosition.x, firstPosition.y};
	checks.aabb.max = { firstPosition.x + checks.BufferSizeX,
		firstPosition.y + checks.BufferSizeY };

	otherChecks.aabb.min = { secondPosition.x, secondPosition.y };
	otherChecks.aabb.max = { secondPosition.x + otherChecks.BufferSizeX,
		secondPosition.y + otherChecks.BufferSizeY };

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
