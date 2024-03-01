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
#include <glad/glad.h>
#include <cassert>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "BaseSystem.h"
#include "PlatformSystem.h"

#define SCREEN_SIZE_X 240
#define SCREEN_SIZE_Y 136

// singleton instance
std::unique_ptr<PlatformSystem> PlatformSystem::instance = nullptr;

Engine::EngineCode PlatformSystem::Init()
{
	// init SDL library
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
	}

	winHandle = SDL_CreateWindow("LanternGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (SCREEN_SIZE_X * 6), (SCREEN_SIZE_Y * 6), SDL_WINDOW_OPENGL);

	if (!winHandle)
	{
		SDL_Quit();
		return Engine::NullWindowHandle;
	}

	// NOTE(thomas): Creating the ONE-AND-ONLY OpenGL context
	oglContext = SDL_GL_CreateContext(winHandle);
	SDL_GL_SetSwapInterval(0);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	// Initialize ImGui
#ifdef _DEBUG
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsLight();
	ImGui_ImplSDL2_InitForOpenGL(winHandle, oglContext);
	ImGui_ImplOpenGL3_Init("#version 330");
#endif

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
#ifdef _DEBUG
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif
	SDL_GL_DeleteContext(oglContext);

	SDL_DestroyWindow(winHandle);
	SDL_Quit();

	return Engine::NothingBad;
}

// get the singleton instance
PlatformSystem* PlatformSystem::GetInstance()
{
	if (!instance)
	{
		instance.reset(new PlatformSystem());
	}
	return instance.get();
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
}