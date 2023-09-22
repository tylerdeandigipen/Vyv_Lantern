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
	typedef Engine::EngineCode(*SceneCode)(void);
	typedef void (*VoidFloat)(float);
	typedef void (*VoidVoid)(void);
public:
	Scene(const char* _name, SceneCode load, SceneCode init, VoidFloat update, VoidVoid render, SceneCode exit, SceneCode unload);
	const char* name;
	enum Mode { Pause, Play };
	Mode mode;
	SceneCode Load;
	SceneCode Init;
	VoidFloat Update;
	VoidVoid FixedUpdate;
	VoidVoid Render;
	SceneCode Exit;
	SceneCode Unload;
};