//------------------------------------------------------------------------------
//
// File Name:	BaseObjects.h
// Author(s):	TayLee Young
// Purpose:		Object class declaration for lights.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#include "Transform.h"
#include "Sprite.h"

class BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject() = default;

	virtual void Update(float dt);
	virtual void Render();

	Transform& GetTrans();
	Sprite& GetSprite();

private:
	Transform transform;
	Sprite sprite;
};