#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
	totalParticles = 0;
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particleArray[i] = NULL;
	}
}
ParticleManager::~ParticleManager()
{
	ClearParticles();
}


void ParticleManager::UpdateParticles()
{
	//Loop through each particle here
	for (int i = 0; i < totalParticles; i++)
	{
		if (particleArray[i]->isDead == true)
		{
			delete particleArray[i];
			particleArray[i] = NULL;
		}
		else if (particleArray[i] != NULL)
		{
			particleArray[i]->Update();
		}
	}
}

void ParticleManager::AddParticle(Particle* particle_)
{
	for (int i = 0; i < totalParticles - 1; i++)
	{
		if (particleArray[i] == NULL)
		{
			particleArray[i] = particle_;
			return;
		}
	}
	particleArray[totalParticles] = particle_;
	totalParticles += 1;
}

void ParticleManager::ClearParticles()
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (particleArray[i] == NULL)
		{
			delete particleArray[i];
		}
	}
	totalParticles = 0;
}