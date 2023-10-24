//------------------------------------------------------------------------------
//
// File Name:	ComponentFactory.cpp
// Author(s):	Michael Howard
// Purpose:		Factory to construct Entities(GameObjects)
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "EntityFactory.h"

EntityFactory* EntityFactory::instance = new EntityFactory();

EntityFactory::EntityFactory()
{

}

EntityFactory::~EntityFactory()
{
    delete instance;
}

Engine::EngineCode EntityFactory::Init()
{
    Add(Entity::ObjectName(), &EntityFactory::CreateObject);
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
    return instance;
}

Entity* EntityFactory::CreateEntity(std::string const& type, const std::string file, json thing)
{
    if (entity_map.count(type))
    {
        return &entity_map[type](type, file, thing);
    }
    else
        return NULL;
}


Engine::EngineCode EntityFactory::Close()
{
    assert(instance != NULL);
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
