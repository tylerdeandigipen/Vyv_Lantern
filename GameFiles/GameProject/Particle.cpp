#include "Particle.h"

Particle::Particle(float x, float y, Color otherColor)
{
	position = {x,y};
	color = otherColor;
}
Particle::Particle() 
{
	position = {0,0};
	color = {0,0,0,0};
}
void Particle::Update()
{
	//move particle here lmao
}

Particle& Particle::operator =(Particle const& rhs)& {
	position = rhs.position;
	color = rhs.color;
	return *this;
};
