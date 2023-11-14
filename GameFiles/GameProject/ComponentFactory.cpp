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
#include "BehaviorDoor.h"
#include "BehaviorSwitch.h"
#include "Physics.h"
#include "Light.h"

ComponentFactory* ComponentFactory::instance = new ComponentFactory();

ComponentFactory::ComponentFactory()
{

}

ComponentFactory::~ComponentFactory()
{
}

Engine::EngineCode ComponentFactory::Init()
{
    Add(BehaviorPlayer::Name(), &ComponentFactory::CreateBehaviorPlayer);
    Add(BehaviorSwitch::Name(), &ComponentFactory::CreateBehaviorSwitch);
    Add(BehaviorDoor::Name(), & ComponentFactory::CreateBehaviorDoor);
    Add(ColliderAABB::Name(), &ComponentFactory::CreateColliderAABB);
    Add(Transform::Name(), &ComponentFactory::CreateTransform);
    Add(Physics::Name(), &ComponentFactory::CreatePhysics);
    Add(Light::TheName(), &ComponentFactory::CreateLight);
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
    delete instance;
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

Component& ComponentFactory::CreateBehaviorDoor(void)
{
    Behavior* behavior = new BehaviorDoor();
    return *behavior;
}

Component& ComponentFactory::CreateBehaviorSwitch()
{
    Behavior* behaviorSwitch = new BehaviorSwitch();
    return *behaviorSwitch;
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

Component& ComponentFactory::CreatePhysics()
{
    Component* physics = new Physics();
    return *physics;
}


Component& ComponentFactory::CreateLight()
{
    Component* light = new Light();
    return *light;
}
