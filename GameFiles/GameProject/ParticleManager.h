#pragma once
#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H
#include "Particle.h"

#define MAX_PARTICLES 100

class ParticleManager
{
public:
	void UpdateParticles();
	void RenderParticles();
	void AddParticle(Particle* particle_);

	Particle* particleArray[MAX_PARTICLES];

private:
	int totalParticles;
};

#endif 
