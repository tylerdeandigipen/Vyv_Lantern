//------------------------------------------------------------------------------
//
// File Name:	ComponentFactory.cpp
// Author(s):	Michael Howard
// Purpose:		Factory to construct Entities(GameObjects)
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "EntityFactory.h"
#include "Entity.h"

std::unique_ptr<EntityFactory> EntityFactory::instance = nullptr;

EntityFactory::EntityFactory() : name("EntFact")
{
}

EntityFactory::~EntityFactory()
{
}

Engine::EngineCode EntityFactory::Init()
{
	Add(Entity::ObjectName(), &EntityFactory::CreateObject);

	// Add(Entity::LightName(), &EntityFactory::CreateLight);
	 //entity_map.emplace(Entity::LightName(), &EntityFactory::CreateLight);
	assert(winHandle != NULL);
	return Engine::NothingBad;
}

void EntityFactory::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void EntityFactory::Render() { }

EntityFactory* EntityFactory::GetInstance()
{
	if (!instance)
	{
		instance.reset(new EntityFactory());
	}
	return instance.get();
}

Entity* EntityFactory::CreateEntity(std::string const& type, const std::string file, json thing)
{
	//if (entity_map.count(type))
	//{
	return &entity_map[type](type, file, thing);

	//}
	//else
	//    return NULL;
}

Engine::EngineCode EntityFactory::Close()
{
	return Engine::NothingBad;
}

void EntityFactory::Add(std::string type, std::function<Entity& (std::string type, const std::string, json thing)> create)
{
	entity_map.emplace(type, create);
}

Entity& EntityFactory::CreateObject(std::string type, const std::string file, json thing)
{
	Entity* entity = new Entity(type, file, thing);
	return *entity;
}

//Entity& EntityFactory::CreateLight(std::string type, const std::string file, json thing)
//{
//    Entity* entity = new Entity(type, file, thing);
//    return *entity;
//}