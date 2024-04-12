//------------------------------------------------------------------------------
//
// File Name:	ParticleManager.cpp
// Author(s):	Tyler Dean
// Purpose:		ParitcleManager class implementation for managing particles.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "ParticleManager.h"
#include "Time_TM.h"

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
	float dt = Time::Instance().GetDt();
#pragma omp parallel for
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
				particleArray[i]->tileMapSize = tileMapSize;
				particleArray[i]->Update(dt);
			}
		}
	}
}

void ParticleManager::AddParticle(Particle* particle_)
{
	if (totalParticles < MAX_PARTICLES)
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