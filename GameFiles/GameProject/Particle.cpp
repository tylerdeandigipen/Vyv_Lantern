#include "Particle.h"

Particle::Particle(Vector2 pos,Vector2 dir, Vector2 spd, Color otherColor, Particle_Type partType)
{
	particleType = partType;
	position = pos;
	direction = dir;
	speed = spd;
	color = otherColor;
	isDead = false;
}
Particle::Particle() 
{
	particleType = Particle_Dust;
	position = {0,0};
	direction = {0,0};
	speed = {0,0};
	color = {0,0,0,0};
	isDead = false;
}
#define RAND_RANGE 12
void Particle::Update()
{
	switch (particleType)
	{
		case Particle_Moth:
		{
			Vector2 temp = { (float)(rand() % RAND_RANGE), (float)(rand() % RAND_RANGE) };
			temp -= {RAND_RANGE / 2, RAND_RANGE / 2};
			temp /= RAND_RANGE;
			direction += temp;
			position.x += cos(direction.x) * speed.x;
			position.y += sin(direction.y) * speed.y;
		}
		break;
		case Particle_Dust:
		{
			Vector2 temp = { (float)(rand() % RAND_RANGE), (float)(rand() % RAND_RANGE) };
			temp -= {RAND_RANGE / 2, RAND_RANGE / 2};
			temp /= RAND_RANGE;
			direction += temp;
			position.x += cos(direction.x) * speed.x;
			position.y += sin(direction.y) * speed.y;
		}
		break;
	}
}

Particle& Particle::operator =(Particle const& rhs)& {
	position = rhs.position;
	color = rhs.color;
	return *this;
};
