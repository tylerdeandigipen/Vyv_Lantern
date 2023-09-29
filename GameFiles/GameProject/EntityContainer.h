#pragma once
#ifndef ENTITYCONTAINER_H
#define ENTITYCONTAINER_H

#include <vector>

typedef class Entity;

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
private:
	std::vector<Entity*> entities;
};

#endif // !ENTITYCONTAINER_H