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
		if (particleArray[i] != NULL)
		{
			if (particleArray[i]->isDead)
			{
				delete particleArray[i];
				particleArray[i] = NULL;
			}
			else
			{
				particleArray[i]->Update();
			}
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
		if (particleArray[i] != NULL)
		{
			delete particleArray[i];
			particleArray[i] = NULL;
		}
	}
	totalParticles = 0;
}

Particle** ParticleManager::GetParticles()
{
	return particleArray;
}