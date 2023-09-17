//------------------------------------------------------------------------------
//
// File Name:	Entity.h
// Author(s):	Michael Howard
// Purpose:		Base class for game objects
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Component.h"
#include <vector>


class Entity
{
public:
	Entity();
	Entity(Entity const& entity);
	~Entity();
	Entity* Clone();

	bool IsNamed(const char* name);
	void Read(Stream stream);
	void FreeComponents();
	void Add(Component*);
	bool IsDestroyed();
	void Destroy();

	void SetName(const char* name);
	
	Component* Get(Component::TypeEnum type) const;
	const char* GetName();
	const char* GetName() const;

	template<typename type_>
	type_* GetComponent(Component::TypeEnum typeId) const
	{
		return static_cast<type_*>(Get(typeId));
	}

	void Update(float dt);
	void Render();
#define Has(type) GetComponent<type>(Component::c##type)

private:
	Component* BinarySearch(Component::TypeEnum type) const;

	char name[256];

	bool isDestroyed;

	std::vector<Component*> components;
	
};
