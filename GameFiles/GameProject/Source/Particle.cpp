//------------------------------------------------------------------------------
//
// File Name:	Particle.cpp
// Author(s):	Tyler Dean
// Purpose:		Particle class implementation for handling particles. 
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Particle.h"
#include "Math.h"
#include <math.h>
#include <cmath>
Particle::Particle(Vector2 pos, Vector2 dir, Vector2 spd, Color otherColor, Particle_Type partType, Vector2 attractPoint)
{
	particleType = partType;
	position = pos;
	direction = dir;
	speed = spd;
	mothAttractionPoint = attractPoint;
	color = otherColor;
	isDead = false;
}
Particle::Particle() 
{
	particleType = Particle_Dust;
	position = {0,0};
	direction = {0,0};
	speed = {0,0};
	mothAttractionPoint = {0,0};
	color = {0,0,0,0};
	isDead = false;
}

//credit to u/waterproof on stackoverflow
float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

#define RAND_RANGE_MOTH 12
#define RAND_RANGE_DUST 12
#define PARTICLE_EDGE_BUFFER 10
void Particle::Update(float dt)
{
	switch (particleType)
	{
		case Particle_Moth:
		{
			/* For the life of me i cant figure out how to make moth go moth things
			float smoothVal = 0.05f;
			Vector2 newDir{0,0};
			float dt = Time::Instance().GetDt();
			Vector2 temp = {(float)(rand() % RAND_RANGE_MOTH), (float)(rand() % RAND_RANGE_MOTH)};
			temp -= {RAND_RANGE_MOTH / 2, RAND_RANGE_MOTH / 2};
			temp /= RAND_RANGE_MOTH;

			newDir += temp * 2;
			newDir.x = cos(newDir.x) * speed.x * dt;
			newDir.y = sin(newDir.y) * speed.y * dt;

			if (distance(mothAttractionPoint.x, mothAttractionPoint.y, position.x, position.y) > 16)
			{
				temp = (mothAttractionPoint - position);
				newDir /= newDir.length(newDir);

				newDir.x += lerp(newDir.x, temp.x, smoothVal);
				newDir.y += lerp(newDir.y, temp.y, smoothVal);
				newDir /= 2;
			}

			direction.x = lerp(direction.x, newDir.x, 0.25f);
			direction.y = lerp(direction.y, newDir.y, 0.25f);
			position.x += (direction.x * speed.x) * dt;
			position.y += (direction.y * speed.y) * dt;
			*/
		}
		break;
		case Particle_Dust:
		{
			Vector2 temp = { (float)(rand() % RAND_RANGE_DUST), (float)(rand() % RAND_RANGE_DUST) };
			temp -= {RAND_RANGE_DUST / 2, RAND_RANGE_DUST / 2};
			temp /= RAND_RANGE_DUST;
			direction += temp;

			position.x += cosf(direction.x) * speed.x * dt;
			position.y += (0.5f + sinf(direction.y)) * speed.y * dt;

			//screen wrapping for dust
			if (position.y > tileMapSize.y + PARTICLE_EDGE_BUFFER)
			{
				position.y = 0;
			}
			if (position.y < 0 - PARTICLE_EDGE_BUFFER)
			{
				position.y = tileMapSize.y;
			}
			if (position.x > tileMapSize.x + (PARTICLE_EDGE_BUFFER / 2) || position.x < 0 - (PARTICLE_EDGE_BUFFER / 2))
			{
				position.x = (float)(rand() % (int)tileMapSize.x - 50);
				position.x += 50;
			}
		}
		break;
	}
}

Particle& Particle::operator =(Particle const& rhs)& {
	position = rhs.position;
	color = rhs.color;
	return *this;
};
