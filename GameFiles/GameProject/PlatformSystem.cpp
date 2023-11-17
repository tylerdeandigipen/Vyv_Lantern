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

#define SCREEN_SIZE_X (240 * 6)
#define SCREEN_SIZE_Y (136 * 6)

// singleton instance
PlatformSystem* PlatformSystem::instance = new PlatformSystem();

Engine::EngineCode PlatformSystem::Init()
{
    // init SDL library 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) 
    {
	}

    winHandle = SDL_CreateWindow("LanternGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_OPENGL);

    if (!winHandle)
    {
        SDL_Quit();
        return Engine::NullWindowHandle;
    }

	assert(winHandle != NULL);
    return Engine::NothingBad;
}

void PlatformSystem::Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
}

void PlatformSystem::Render()
{ 
}

// Close it all down
Engine::EngineCode PlatformSystem::Close()
{
    assert(instance != NULL);
    SDL_DestroyWindow(winHandle);
    SDL_Quit();
    return Engine::NothingBad;
}

// get the singleton instance
PlatformSystem* PlatformSystem::GetInstance()
{
    return instance;
}

void PlatformSystem::ChangeTitle(const char* title)
{
    assert(title != NULL);
    SDL_SetWindowTitle(winHandle, title);
}

SDL_Window* PlatformSystem::GetWindowHandle()
{
    return winHandle;
}

PlatformSystem::PlatformSystem() : BaseSystem("PlatformSystem"), winHandle(NULL) {}
PlatformSystem::~PlatformSystem()
{
    if (instance)
        delete instance;
}
