//------------------------------------------------------------------------------
//
// File Name:	Collision.h
// Author(s):	Louis Wang
// Purpose:		Declaration of AABB collision checking.
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#ifndef COLLISION_H_
#define COLLISION_H_
#include "Vector.h"
/*Objects inherit AABB struct to have min and max values(Bounding boxes), then function collisionCheck to check whether
they collides with other objects*/
struct AABB
{
	/*minimum and maximum of x and y*/
	gfxVector2 min, max;
};

/*Check 2 boxes' min and max, 4 cases and if ANY of the case returns 1 means its colliding SOMEWHERE, 0 for no collision*/
bool CollisionCheck(const AABB& aabb1, const AABB aabb2);

bool CollisionCheck(const AABB& aabb, Vector2 mousePos);

#endif