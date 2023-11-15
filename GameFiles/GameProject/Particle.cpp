#include "Particle.h"
#include "Time.h"
#include "Math.h"

Particle::Particle(Vector2 pos,Vector2 dir, Vector2 spd, Vector2 attractPoint, Color otherColor, Particle_Type partType)
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

#define RAND_RANGE 12
#define PARTICLE_EDGE_BUFFER 10
void Particle::Update()
{
	switch (particleType)
	{
		case Particle_Moth:
		{
			float dt = Time::Instance().GetDt();

			Vector2 temp = {(float)(rand() % RAND_RANGE), (float)(rand() % RAND_RANGE)};
			temp -= {RAND_RANGE / 2, RAND_RANGE / 2};
			temp /= RAND_RANGE;
			direction += temp;
			temp = (mothAttractionPoint - position);
			temp /= 100.0f;
			direction += temp;

			position.x += cos(direction.x) * speed.x * dt;
			position.y += sin(direction.y) * speed.y * dt;
		}
		break;
		case Particle_Dust:
		{
			float dt = Time::Instance().GetDt();

			Vector2 temp = { (float)(rand() % RAND_RANGE), (float)(rand() % RAND_RANGE) };
			temp -= {RAND_RANGE / 2, RAND_RANGE / 2};
			temp /= RAND_RANGE;
			direction += temp;

			position.x += cos(direction.x) * speed.x * dt;
			position.y += (0.5 + sin(direction.y)) * speed.y * dt;

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
