#pragma once
#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H
#include "Particle.h"

#define MAX_PARTICLES 150

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	void UpdateParticles();
	void AddParticle(Particle* particle_);
	void ClearParticles();

	Particle* particleArray[MAX_PARTICLES];
	int totalParticles;

	Particle** GetParticles();
	Vector2 tileMapSize{ 240,136 };
};

#endif 
