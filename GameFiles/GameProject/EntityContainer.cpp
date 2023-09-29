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
#include "Entity.h"

EntityContainer::EntityContainer() : entities(NULL)
{

}

EntityContainer::~EntityContainer()
{
}

bool EntityContainer::AddEntity(Entity* entity)
{
	entities.push_back(entity);
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
//		Collider* collider = entities[current]->Has(Collider);
//		if (collider)
//		{
// 		   for (unsigned i = current + 1; i < entities.size(); ++i)
// 		   {
//				if (entities[i])
// 				{
//					Collider* secCollider = entities[i]->Has(Collider);
//					if (secCollider)
//					{
//						collider->Check(secCollider);
//					}
// 				}
// 		   }
//		}
		++current;
	}
}

void EntityContainer::UpdateAll(float dt)
{
	for (unsigned i = 0; entities.size(); ++i)
	{
		if (entities[i])
		{
			entities[i]->Update(dt);
			if (entities[i]->IsDestroyed())
			{
				entities[i]->FreeComponents();
				delete entities[i];
				entities[i] = NULL;
				
			}
		}
	}
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
	for (unsigned i = 0; entities.max_size(); ++i)
	{
		if (entities[i] != NULL)
		{
			entities[i]->FreeComponents();
			delete entities[i];
			entities[i] = NULL;
		}
	}
	entities.clear();
}