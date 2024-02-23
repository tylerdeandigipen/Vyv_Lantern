//------------------------------------------------------------------------------
//
// File Name:	EntityFactory.h
// Author(s):	Michael Howard
// Purpose:		EntityFactory class declaration for creating entities.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "BaseSystem.h"
#include <nlohmann/json.hpp>

class Entity;
using json = nlohmann::json;

class EntityFactory : public BaseSystem
{
public:
	EntityFactory();
	~EntityFactory();

	BaseSystem& operator=(BaseSystem const&) = delete;

	Engine::EngineCode Init();
	void Update(float dt);
	void Render();
	std::string ObjectName();

	//std::string LightName();
	static EntityFactory* GetInstance();
	Entity* CreateEntity(std::string const& type, const std::string file, json thing = NULL);
	Engine::EngineCode Close();
private:
	static Entity& CreateObject(std::string, const std::string file, json thing = NULL);

	//static Entity& CreateLight(std::string type, const std::string file, json thing = NULL);
	//static Entity& CreateLight(void);
	static EntityFactory* instance;
	std::unordered_map<std::string, std::function<Entity& (std::string, const std::string, json)>> entity_map;
	const char* name;
	void Add(std::string name, std::function<Entity& (std::string, const std::string, json)> create);
};

