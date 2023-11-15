#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vector.h"
#include "Color.h"

enum Particle_Type
{
	Particle_Moth,
	Particle_Dust
};

class Particle
{
public:
	Particle(Vector2 pos, Vector2 dir, Vector2 spd, Vector2 attractPoint, Color otherColor, Particle_Type partType);
	Particle();
	void Update();

	Particle& operator =(Particle const& rhs)&;

	enum Particle_Type particleType = Particle_Dust;
	Vector2 position;
	Vector2 direction;
	Vector2 speed;
	Color color;
	bool isDead = false;
	Vector2 mothAttractionPoint{ 0,0 };
	Vector2 tileMapSize{240,136};
};

#endif 
