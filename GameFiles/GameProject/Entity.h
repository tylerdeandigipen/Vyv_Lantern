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
#include "ImageBuffer.h"
#include "Inputs.h"
#include "Renderer.h"
#include <vector>
#include <SDL/SDL.h>


class Entity
{
public:
	Entity();
	Entity(const char* file, SDL_Window* _window);
	Entity(Entity const& entity);
	~Entity();
	Entity* Clone();

	void CreateImage(const char* file);

	bool IsNamed(const char* name);
	void Read(FILE* stream);
	void FreeComponents();
	void Add(Component*);
	void AddToRenderer(Renderer* pixel);
	bool IsDestroyed();
	void Destroy();

	void SetName(const char* name);
	void SetWindow(SDL_Window* _window);
	void SetInputHandler(Inputs* input);

	SDL_Window* GetWindow();
	
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

	SDL_Window* window;

private:
	Component* BinarySearch(Component::TypeEnum type) const;

	ImageBuffer* image;

	char name[256];

	bool isDestroyed;


	std::vector<Component*> components;
	
};
