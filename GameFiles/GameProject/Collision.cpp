#include "Collision.h"

bool CollisionCheck(const AABB& aabb1, const AABB aabb2)
{
	/*check for collision*/
	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x ||
		aabb1.max.y <aabb2.min.y || aabb1.min.y > aabb2.min.y)
	{
		return 0;
	}
	else
	{
		/*there is collision*/
		return 1;
	}
}