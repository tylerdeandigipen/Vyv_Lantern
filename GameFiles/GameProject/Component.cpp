//------------------------------------------------------------------------------
//
// File Name:	Component.cpp
// Author(s):	Michael Howard (michael.howard)
// Purpose:		(Borrowed from cs230) creates a base componenet class for 
//				entities
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Component.h"

Component::Component(Component::TypeEnum _type) : mType(_type), mParent(nullptr)
{
}

Component::Component(Component const& other) : mType(other.mType), mParent(other.mParent)
{
}

Component::~Component()
{
}



