//------------------------------------------------------------------------------
//
// File Name:	ComponentFactory.h
// Author(s):	Michael Howard
// Purpose:		Factory to construct components
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "BaseSystem.h"
#include "Component.h"

class ComponentFactory : public BaseSystem
{
public:
	ComponentFactory();
	//ComponentFactory(const char* _name = "");
	~ComponentFactory();
	
	BaseSystem& operator=(BaseSystem const&) = delete;

	Engine::EngineCode Init();
	void Update(float dt);
	void Render();
	static ComponentFactory* GetInstance();
	Component* CreateComponent(std::string const& type);
	Engine::EngineCode Close();
private:
	static Component& CreateBehaviorPlayer(void);
	static Component& CreateColliderAABB(void);
	static Component& CreateTransform(void);
	//static Component& CreateLight(void);
	static ComponentFactory* instance;
	std::unordered_map<std::string, std::function<Component&()>> component_map;
	const char* name;
	void Add(std::string name, std::function<Component&()> create);
};