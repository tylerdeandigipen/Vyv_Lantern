//------------------------------------------------------------------------------
//
// File Name:	ComponentFactory.h
// Author(s):	Michael Howard
// Purpose:		Factory to construct components
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "BaseSystem.h"

class Component;

class ComponentFactory : public BaseSystem
{
public:

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
	ComponentFactory();
	static Component& CreateBehaviorPlayer(void);
	static Component& CreateBehaviorSwitch(void);
	static Component& CreateBehaviorMirror(void);
	static Component& CreateBehaviorEmitter();
	static Component& CreateBehaviorReceiver();
	static Component& CreateColliderAABB(void);
	static Component& CreateBehaviorDoor(void);
	static Component& CreateTransform(void);
	static Component& CreatePhysics(void);
	static Component& CreateLight(void);
	static Component& CreateEmitter(void);
	static Component& CreateLineCollider(void);
	static std::unique_ptr<ComponentFactory> instance;
	std::unordered_map<std::string, std::function<Component& ()>> component_map;
	const char* name;
	void Add(std::string name, std::function<Component& ()> create);
};