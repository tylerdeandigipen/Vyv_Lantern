//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.cpp
// Author(s):	Michael Howard
// Purpose:		Entity Container
//				(adapted from Schillings usage in cs230)
//				(added std::vector usage)
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "EntityContainer.h"
#include "Collider.h"
#include "Entity.h"

int EntityContainer::entityCount = 0;

EntityContainer::EntityContainer() : entities()
{

}

EntityContainer::~EntityContainer()
{

}

bool EntityContainer::AddEntity(Entity* entity)
{
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

int EntityContainer::CountEntities()
{
	return entities.size();
}