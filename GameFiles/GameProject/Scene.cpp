//------------------------------------------------------------------------------
//
// File Name:	PlatformSystem.c
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		base class for scenes
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Scene.h"

Scene::Scene(const char* _name, SceneCode load, SceneCode init, VoidFloat update, VoidVoid render, SceneCode exit, SceneCode unload)
	: Load(load), Init(init), Update(update), Render(render), Exit(exit), Unload(unload)
{ 
	Scene::name = _name;
	Scene::mode = Mode::Play;
}
