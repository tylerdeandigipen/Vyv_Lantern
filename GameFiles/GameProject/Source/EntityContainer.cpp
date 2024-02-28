//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.cpp
// Author(s):	Michael Howard
// Purpose:		Entity Container
//				(adapted from Schillings usage in cs230)
//				(added std::vector usage)
//
// Copyright  ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "EntityContainer.h"
#include "EntityFactory.h"
#include "FileIO.h"
#include "Collider.h"
#include "Entity.h"

//laser "System" stuff
#include "Emitter.h"
#include "LineCollider.h"

int EntityContainer::entityCount = 0;
std::unique_ptr<EntityContainer> EntityContainer::instance = nullptr;

EntityContainer::EntityContainer() : entities()
{
}

EntityContainer::~EntityContainer()
{
}

Engine::EngineCode EntityContainer::Init()
{
	return Engine::NothingBad;
}

void EntityContainer::Update(float dt)
{
	UpdateAll(dt);
}

void EntityContainer::Render()
{
}

void EntityContainer::ReadEntities(std::string filepath)
{
	Renderer* pixel = Renderer::GetInstance();

	json listData = FileIO::GetInstance()->OpenJSON(filepath);

	//if (jsonData["Entities"].is_array())
	//{
	for (auto& entityData : listData["Objects"])
	{
		std::string file = entityData["FilePath"];
		json jsonData = FileIO::GetInstance()->OpenJSON(file); // the file to entire objects data

		Entity* newEnt = FileIO::GetInstance()->ReadEntity(jsonData);
		AddEntity(newEnt);
	}

	//}
}

Engine::EngineCode EntityContainer::Close()
{
	return Engine::NothingBad;
}

bool EntityContainer::AddEntity(Entity* entity)
{
	// add logging info
	entities.push_back(entity);
	++entityCount;
	return true;
}

Entity* EntityContainer::FindByName(const char* name)
{
	for (unsigned i = 0; i < entities.size(); ++i)
	{
		if (entities[i])
			if (entities[i]->IsNamed(name))
				return entities[i];
	}
	return NULL;
}

// allows the ability to loop through and get the information of each entity/game-object
Entity* EntityContainer::operator[](int index)
{
	if (!entities.empty())
	{
		if (entities[index] != NULL)
		{
			return entities[index];
		}
		else
		{
			//add logging info
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

bool EntityContainer::IsEmpty()
{
	if (entities.size() == 0)
		return true;
	else
		return false;
}

void EntityContainer::CheckCollisions()
{
	unsigned current = 0;
	while (current != entities.size())
	{
		Collider* collider = entities[current]->Has(Collider);
		if (collider)
		{
			for (unsigned i = current + 1; i < entities.size(); ++i)
			{
				if (entities[i])
				{
					Collider* secCollider = entities[i]->Has(Collider);
					if (secCollider)
					{
						collider->Check(secCollider);
					}

					if (entities[current]->Has(Emitter) && entities[i]->Has(LineCollider))
					{
						Emitter::EmitterCollisionHandler(*entities[current], *entities[i]);
					}
				}
			}
		}
		++current;
	}
}

void EntityContainer::UpdateAll(float dt)
{
	int i = 0;
	for (auto it : entities)
	{
		if (it)
		{
			(it)->Update(dt);
			if (it->IsDestroyed())
			{
				it->FreeComponents();
				delete it;
				it = NULL;
			}
		}
		++i;
	}
	CheckCollisions();
}

void EntityContainer::RenderAll()
{
	for (unsigned i = 0; i < entities.size(); ++i)
	{
		if (entities[i])
		{
			entities[i]->Render();
		}
	}
}

void EntityContainer::FreeAll()
{
	if (this)
	{
		if (!entities.empty())
		{
			for (auto it : entities)
			{
				if (it)
				{
					it->FreeComponents();
					delete it;
					it = NULL;
				}
			}
			entities.clear();
		}
	}
}

EntityContainer* EntityContainer::GetInstance()
{
	if (!instance)
	{
		instance.reset(new EntityContainer());
	}
	return instance.get();
}

int EntityContainer::CountEntities()
{
	return entities.size();
}

bool EntityContainer::RemoveEntity(Entity* _entity)
{
	auto it = std::find(entities.begin(), entities.end(), _entity);
	if (it != entities.end())
	{
		entities.erase(it);
		--entityCount;
		delete _entity;
		return true;
	}
	else
	{
		return false;
	}
}