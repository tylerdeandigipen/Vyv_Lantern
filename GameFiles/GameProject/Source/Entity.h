//------------------------------------------------------------------------------
//
// File Name:	Entity.h
// Author(s):	Michael Howard
// Purpose:		Base class for game objects
//
// Copyright ?2023 DigiPen (USA) Corporation.
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
	Entity(std::string type, const std::string file = NULL, json Animation = NULL );
	Entity(Entity const& entity);
	~Entity();
	Entity* Clone();

	void CreateImage(const std::string file);
	void CreateAnimatedImage(const std::string);

	bool IsNamed(const char* name);
	bool IsPicked(Renderer* pRenderder, Vector2 mousePos);
	void Read(json &stream);
	void FreeComponents();
	void Add(Component*);
	void AddToRenderer(Renderer* pixel, std::string _file = "");
	bool IsDestroyed();
	void Destroy();

	static std::string ObjectName();

	void SetName(const char* name);
	void SetInputHandler(Inputs* input);

	
	
	Component* Get(Component::TypeEnum type) const;
	std::string GetRealName();
	const char* GetName();
	const char* GetName() const;

	ImageBuffer* GetImage();
	void SetImage(std::string ppm);
	template<typename type_>
	type_* GetComponent(Component::TypeEnum typeId) const
	{
		return static_cast<type_*>(Get(typeId));
	}

	std::string GetFilePath()
	{
		return filePath;
	}

	bool IsObject();
	bool IsAnimated();


	void Update(float dt);
	void Render();
#define Has(type) GetComponent<type>(Component::c##type)
	std::string lightFile;
private:
	Component* BinarySearch(Component::TypeEnum type) const;

	ImageBuffer* image;
	ImageBuffer* AnimationArray;

	std::string filePath;

	char name[256];

	std::string mName;

	bool isAnimated;

	bool isDestroyed;

	bool isLight;

	bool isObject;

	Vector2 FrameSize;

	std::vector<Component*> components;
	
};
