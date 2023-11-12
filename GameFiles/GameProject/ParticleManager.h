#pragma once
#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H
#include "Particle.h"

#define MAX_PARTICLES 100

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

	// thanks ty
	Particle** GetParticles();
};

#endif 
