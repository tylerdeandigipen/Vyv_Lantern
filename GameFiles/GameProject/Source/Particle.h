//------------------------------------------------------------------------------
//
// File Name:	Paritcle.h
// Author(s):	Tyler Dean
// Purpose:		Particle class declaration for handling individual particles.
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vector.h"
#include "Color.h"

enum Particle_Type
{
	Particle_Moth,
	Particle_Dust,
	Particle_Laser_Emiter
};

class Particle
{
public:
	Particle(Vector2 pos, Vector2 dir, Vector2 spd, Color otherColor, Particle_Type partType, float givenLifeTime = 0, Vector2 attractPoint = {0,0});
	Particle();
	void Update(float dt);

	Particle& operator =(Particle const& rhs)&;

	enum Particle_Type particleType = Particle_Dust;
	Vector2 spawnPos;
	Vector2 position;
	Vector2 direction;
	Vector2 speed;
	float lifeTime = 0;
	Color color;
	bool isDead = false;
	Vector2 mothAttractionPoint{ 0,0 };

	//dont touch
	Vector2 tileMapSize{240,136};
	bool isTransparent = false;
	float timeAlive = 0;
};

#endif 
