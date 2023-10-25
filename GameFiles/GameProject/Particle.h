#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vector.h"
#include "Color.h"

class Particle
{
public:
	Particle(float x, float y, Color otherColor);
	Particle();
	void Update();

	Particle& operator =(Particle const& rhs)&;

	Vector2 position;
	Color color;
	bool isDead = false;
};

#endif 
