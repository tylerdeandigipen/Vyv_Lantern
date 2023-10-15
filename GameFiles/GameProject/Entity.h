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
#include "stdafx.h"
#include "Component.h"
#include "ImageBuffer.h"
#include "Inputs.h"
#include "Renderer.h"
#include <vector>
#include <SDL/SDL.h>


class Entity
{
public:
	Entity();
	Entity(std::string type, const std::string file = NULL);
	Entity(Entity const& entity);
	~Entity();
	Entity* Clone();

	void CreateImage(const std::string file);

	bool IsNamed(const char* name);
	void Read(json const& stream);
	void FreeComponents();
	void Add(Component*);
	void AddToRenderer(Renderer* pixel);
	bool IsDestroyed();
	void Destroy();

	static std::string ObjectName();
	//static std::string LightName();

	void SetName(const char* name);
	void SetInputHandler(Inputs* input);

	
	
	Component* Get(Component::TypeEnum type) const;
	const char* GetName();
	const char* GetName() const;

	template<typename type_>
	type_* GetComponent(Component::TypeEnum typeId) const
	{
		return static_cast<type_*>(Get(typeId));
	}

	bool IsLight();
	bool IsObject();

	void Update(float dt);
	void Render();
#define Has(type) GetComponent<type>(Component::c##type)

private:
	Component* BinarySearch(Component::TypeEnum type) const;

	ImageBuffer* image;

	char name[256];

	std::string mName;

	bool isDestroyed;

	bool isLight;

	bool isObject;


	std::vector<Component*> components;
	
};
