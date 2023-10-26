#pragma once
#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H
#include "Particle.h"

#define MAX_PARTICLES 100

class ParticleManager
{
public:
	ParticleManager();
	void UpdateParticles();
	void AddParticle(Particle* particle_);

	Particle* particleArray[MAX_PARTICLES];
	int totalParticles;
};

#endif 
