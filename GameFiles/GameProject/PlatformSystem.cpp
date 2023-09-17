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

#include "BaseSystem.h"
#include "PlatformSystem.h"

PlatformSystem* PlatformSystem::instance = new PlatformSystem();

Engine::EngineCode PlatformSystem::Init()
{
    return Engine::NothingBad;
}

void PlatformSystem::Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
}

void PlatformSystem::Render() { }

Engine::EngineCode PlatformSystem::Close()
{
    return Engine::NothingBad;
}

PlatformSystem* PlatformSystem::GetInstance()
{
    return instance;
}

void PlatformSystem::ChangeTitle(const char* title)
{
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
