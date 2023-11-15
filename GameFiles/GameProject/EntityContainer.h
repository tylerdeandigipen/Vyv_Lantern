//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.h
// Author(s):	Michael Howard
// Purpose:		header
//
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#ifndef ENTITYCONTAINER_H
#define ENTITYCONTAINER_H

#include <vector>

typedef class Entity Entity;

using namespace std;

class EntityContainer
{
public:
	EntityContainer();
	~EntityContainer();
	bool AddEntity(Entity* _entity);
	Entity* FindByName(const char* entity_name);
	bool IsEmpty();
	void UpdateAll(float dt);
	void CheckCollisions();
	void RenderAll();
	void FreeAll();

	int CountEntities();
private:
	static int entityCount;
	std::vector<Entity*> entities;
};

#endif // !ENTITYCONTAINER_H