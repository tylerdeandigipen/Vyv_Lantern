#include "ParticleManager.h"

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
	if (particleArray[totalParticles] == NULL)
	{
		totalParticles -= 1;
	}
}

void ParticleManager::RenderParticles()
{
	//Render each particle here
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