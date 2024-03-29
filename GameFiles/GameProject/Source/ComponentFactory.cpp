//------------------------------------------------------------------------------
//
// File Name:	ComponentFactory.cpp
// Author(s):	Michael Howard, Louis Wang
// Purpose:		Factory to construct components
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "ComponentFactory.h"
#include "ColliderAABB.h"
#include "Transform.h"
#include "BehaviorPlayer.h"
#include "BehaviorDoor.h"
#include "BehaviorSwitch.h"
#include "BehaviorMirror.h"
#include "BehaviorEmitter.h"
#include "BehaviorReceiver.h"
#include "Physics.h"
#include "Light.h"
#include "Emitter.h"
#include "LineCollider.h"
std::unique_ptr<ComponentFactory> ComponentFactory::instance = nullptr;

ComponentFactory::ComponentFactory() : name("ComptFact")
{
}

ComponentFactory::~ComponentFactory()
{
}

Engine::EngineCode ComponentFactory::Init()
{
	Add(BehaviorPlayer::Name(), &ComponentFactory::CreateBehaviorPlayer);
	Add(BehaviorSwitch::Name(), &ComponentFactory::CreateBehaviorSwitch);
	Add(BehaviorDoor::Name(), &ComponentFactory::CreateBehaviorDoor);
	Add(BehaviorMirror::Name(), &ComponentFactory::CreateBehaviorMirror);
	Add(BehaviorEmitter::Name(), &ComponentFactory::CreateBehaviorEmitter);
	Add(BehaviorReceiver::Name(), &ComponentFactory::CreateBehaviorReceiver);
	Add(ColliderAABB::Name(), &ComponentFactory::CreateColliderAABB);
	Add(Transform::Name(), &ComponentFactory::CreateTransform);
	Add(Physics::Name(), &ComponentFactory::CreatePhysics);
	Add(Light::TheName(), &ComponentFactory::CreateLight);
	Add(Emitter::Name(), &ComponentFactory::CreateEmitter);
	Add(LineCollider::Name(), &ComponentFactory::CreateLineCollider);
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
	if (!instance)
	{
		instance.reset(new ComponentFactory());
	}
	return instance.get();
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
	return Engine::NothingBad;
}

void ComponentFactory::Add(std::string named, std::function<Component& (void)> create)
{
	component_map.emplace(named, create);
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

Component& ComponentFactory::CreateBehaviorMirror()
{
	Behavior* behaviorMirror = new BehaviorMirror();
	return *behaviorMirror;
}

Component& ComponentFactory::CreateBehaviorEmitter()
{
	Behavior* behaviorEmitter = new BehaviorEmitter();
	return *behaviorEmitter;
}

Component& ComponentFactory::CreateBehaviorReceiver()
{
	Behavior* behaviorReceiver = new BehaviorReceiver();
	return *behaviorReceiver;
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

Component& ComponentFactory::CreateEmitter()
{
	Component* emitter = new Emitter();
	return *emitter;
}

Component& ComponentFactory::CreateLineCollider()
{
	Component* lineCollider = new LineCollider();
	return *lineCollider;
}