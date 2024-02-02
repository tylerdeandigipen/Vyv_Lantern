//------------------------------------------------------------------------------
//
// File Name:	PlatformSystem.h
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		base class for scenes
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#include "Engine.h"

class Scene
{
private:
//	typedef Engine::EngineCode(*SceneCode)(void);
//	typedef void (*VoidFloat)(float);
//	typedef void (*VoidVoid)(void);
public:
	Scene(const char* name);
	const char* name;
	enum Mode { Pause, Play };
	Mode mode;
	virtual Engine::EngineCode Load() = 0;
	virtual Engine::EngineCode Init() = 0;
	virtual void Update(float dt) {};
	virtual void Render() {};
	virtual Engine::EngineCode Exit() = 0;
	virtual Engine::EngineCode Unload() = 0;
};