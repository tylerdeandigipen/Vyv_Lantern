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

typedef class Entity Entity;
typedef FILE* Stream;

class Component
{
public:

	enum TypeEnum { cBehavior, cCollider, cPhysics, cImageBuffer, cTransform };
	_inline Component::TypeEnum type() const { return mType; };

	_inline void Parent(Entity* entity) { mParent = entity; }
	_inline Entity* Parent() const { return mParent; }

	Component(Component::TypeEnum type);
	Component(Component const& other);
	virtual ~Component();

	// used to invoke the copy constructor
	virtual Component* Clone(void) const = 0;

	//component-specific render code.
	virtual void Update(float dt) { dt = dt; }

	virtual void Render() const {};
	virtual void Read(Stream stream) = 0;

private:
	TypeEnum mType;
	Entity* mParent;
};