//------------------------------------------------------------------------------
//
// File Name:	ComponentFactory.cpp
// Author(s):	Michael Howard
// Purpose:		Factory to construct components
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "ComponentFactory.h"
#include "ColliderAABB.h"
#include "Transform.h"
#include "BehaviorPlayer.h"

ComponentFactory* ComponentFactory::instance = new ComponentFactory();

ComponentFactory::ComponentFactory()
{

}

ComponentFactory::~ComponentFactory()
{
    delete instance;
}

Engine::EngineCode ComponentFactory::Init()
{
    Add(BehaviorPlayer::Name(), &ComponentFactory::CreateBehaviorPlayer);
    Add(ColliderAABB::Name(), &ComponentFactory::CreateColliderAABB);
    Add(Transform::Name(), &ComponentFactory::CreateTransform);
    assert(winHandle != NULL);
    return Engine::NothingBad;
}

void ComponentFactory::Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
}

void ComponentFactory::Render() { }

ComponentFactory* ComponentFactory::GetInstance()
{
    return instance;
}

Component* ComponentFactory::CreateComponent(std::string const& type)
{
    if (component_map.count(type))
    {
        return &component_map[type]();
    }
    else
        return NULL;
}


Engine::EngineCode ComponentFactory::Close()
{
    assert(instance != NULL);
    return Engine::NothingBad;
}

void ComponentFactory::Add(std::string name, std::function<Component& (void)> create)
{
    component_map.emplace(name, create);
}



Component& ComponentFactory::CreateBehaviorPlayer()
{
    Behavior* behavior = new BehaviorPlayer();
    return *behavior;
}

Component& ComponentFactory::CreateColliderAABB(void)
{
    Collider* collider = new ColliderAABB();
    return *collider;
}

Component& ComponentFactory::CreateTransform()
{
    Component* transform = new Transform();
    return *transform;
}

//Component& ComponentFactory::CreateLight()
//{
//    Light* light = new Light();
//    return *light;
//}
