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

Scene::Scene(const char* _name)
{ 
	name = _name;
	mode = Mode::Play;
}


