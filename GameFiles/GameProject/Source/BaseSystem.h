//------------------------------------------------------------------------------
//
// File Name:	BaseSystem.h
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		Base class for all systems
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include "Engine.h"

class BaseSystem
{
public:
	BaseSystem(const char* _name = "");
	BaseSystem(BaseSystem const&) = delete;
	BaseSystem& operator=(BaseSystem const&) = delete;

	virtual Engine::EngineCode Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual Engine::EngineCode Close() = 0;

	virtual ~BaseSystem() = default;
private:
	const char* name;
};