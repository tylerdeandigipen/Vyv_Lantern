#pragma once
#include "stdafx.h"
#include "BaseSystem.h"
#include "Entity.h"

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
	Entity* CreateEntity(std::string const& type, const std::string file);
	Engine::EngineCode Close();
private:
	static Entity& CreateObject(std::string, const std::string file);
	//static Entity& CreateLight(void);
	static EntityFactory* instance;
	std::unordered_map<std::string, std::function<Entity& (std::string, const std::string)>> entity_map;
	const char* name;
	void Add(std::string name, std::function<Entity& (std::string, const std::string)> create);
};

