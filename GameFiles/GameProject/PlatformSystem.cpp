//------------------------------------------------------------------------------
//
// File Name:	PlatformSystem.cpp
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		handles systems (thumbs up)
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Windows.h>
#include <SDL/SDL.h>
#include <cassert>

#include "BaseSystem.h"
#include "PlatformSystem.h"

PlatformSystem* PlatformSystem::instance = new PlatformSystem();

Engine::EngineCode PlatformSystem::Init()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		// @TODO: Log failuer to initialize SDL
	}

	assert(winHandle != NULL);
    return Engine::NothingBad;
}

void PlatformSystem::Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
}

void PlatformSystem::Render() { }

Engine::EngineCode PlatformSystem::Close()
{
    assert(instance != NULL);
    return Engine::NothingBad;
}

PlatformSystem* PlatformSystem::GetInstance()
{
    return instance;
}

void PlatformSystem::ChangeTitle(const char* title)
{
    assert(title != NULL);
    SetWindowTextA(winHandle, title);
}

int PlatformSystem::GetHeight() { return windowHeight; }
int PlatformSystem::GetWidth() { return windowWidth; }

PlatformSystem::PlatformSystem() : BaseSystem("PlatformSystem"), windowHeight(600), windowWidth(960), winHandle(NULL) {}
PlatformSystem::~PlatformSystem()
{
    if (instance)
        delete instance;
}
