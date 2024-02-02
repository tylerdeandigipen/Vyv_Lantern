//------------------------------------------------------------------------------
//
// File Name:	Component.h
// Author(s):	Michael Howard (michael.howard)
// Purpose:		(Borrowed from cs230) creates a base componenet class for 
//				entities
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include <nlohmann/json.hpp>

class Entity;

using json = nlohmann::json;

//typedef FILE* Stream;

class Component
{
public:

	enum TypeEnum { cBehavior, cCollider, cImageBuffer, cLight, cPhysics, cTransform, cSprite, cAnimation, cMirror, cSwitch, cLazer, cDoor};
	_inline Component::TypeEnum type() const { return mType; };

	_inline void Parent(Entity* entity) { mParent = entity; }
	_inline Entity* Parent() const { return mParent; }

	Component(Component::TypeEnum type);
	Component(Component const& other);
	virtual std::string GetName() { return "Component"; };
	//virtual CreateComponent(mType);
	virtual ~Component();

	// used to invoke the copy constructor
	virtual Component* Clone(void) const = 0;

	//component-specific render code.
	virtual void Update(float dt) { dt = dt; }

	virtual void Render() const {};
	virtual void Read(json jsonData) = 0;

private:
	TypeEnum mType;
	Entity* mParent;
};