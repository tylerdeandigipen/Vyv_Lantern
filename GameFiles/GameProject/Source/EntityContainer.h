//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.h
// Author(s):	Michael Howard
// Purpose:		Entity Container class definition.
//
//
// Copyright  � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#ifndef ENTITYCONTAINER_H
#define ENTITYCONTAINER_H

#include "BaseSystem.h"
#include <vector>

typedef class Entity Entity;

using namespace std;

class EntityContainer : public BaseSystem
{
public:
	~EntityContainer();
	bool AddEntity(Entity* _entity);
	Entity* FindByName(const char* entity_name);
	Entity* operator[](int index);
	bool IsEmpty();
	void UpdateAll(float dt);
	void CheckCollisions();
	void RenderAll();
	void FreeAll();
	bool RemoveEntity(Entity* _entity);

	Engine::EngineCode Init();
	void Update(float dt);
	void Render();
	void ReadEntities(std::string filepath);
	Engine::EngineCode Close();
	static EntityContainer* GetInstance();

	int CountEntities();
private:
	EntityContainer();
	static std::unique_ptr<EntityContainer> instance;
	static int entityCount;
	std::vector<Entity*> entities;
};

#endif // !ENTITYCONTAINER_H