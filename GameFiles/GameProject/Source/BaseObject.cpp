//------------------------------------------------------------------------------
//
// File Name:	BaseObject.cpp
// Author(s):	TayLee Young
// Purpose:		Base class for all objects
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "BaseObject.h"

BaseObject::BaseObject() : transform(), sprite()
{
}

void BaseObject::Update(float dt)
{
   
}

void BaseObject::Render()
{
   
}

Transform& BaseObject::GetTrans()
{
    return transform;
}

Sprite& BaseObject::GetSprite()
{
    return sprite;
}
