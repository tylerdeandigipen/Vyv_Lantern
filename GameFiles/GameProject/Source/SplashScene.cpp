//------------------------------------------------------------------------------
//
// File Name:	SplashScene.cpp
// Author(s):	
// Purpose:		Change this to whatever you need
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "SplashScene.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Engine.h"
#include <SDL/SDL.h>
#include "Transform.h"
#include "PlatformSystem.h"
#include "EntityContainer.h"
#include "TbdTestScene.h"
#include "LevelBuilder.h"
Scene* SplashSceneinstance = NULL; //SORRY MIKEY L MEYERS!!!!!!!!!!!!!!!!!!!!!!

SDL_Window* SplashWindow;

SDL_GLContext SplashGlContext;


SplashScene::SplashScene() : Scene("Splash")
{
	time = 3.0f;
	logo = nullptr;	
	entityManagerSPL = std::make_unique<EntityManager>();

}

SplashScene::~SplashScene()
{

}

Engine::EngineCode SplashScene::Init()
{
	Renderer::GetInstance()->isFullbright = true;
	//TODO make a LOGO sprite
		// Create SDL Window
	SplashWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	Renderer::GetInstance()->window = SplashWindow;

	LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/Splash/Splash.json");

	return Engine::NothingBad;
}

Engine::EngineCode SplashScene::Load()
{

	if (entityManagerSPL->InitializeProperties("./Data/GameObjects/ObjectList.json"))
		std::cout << "Property load success!\n";

	return Engine::NothingBad;
}

void SplashScene::Update(float dt)
{
	if (CheckGameScenes() || CheckRestart())
	{
		return;
	}
	
	if (time <= 0.0f)
	{
		//TODO get the menu scene.
		Renderer::GetInstance()->isFullbright = false;
		SceneSystem::GetInstance()->SetScene(TbdTestSceneGetInstance());
		ImGuiInterg();

	}
	else
	{
		time -= dt;
	}

	EntityContainer::GetInstance()->UpdateAll(dt);
	Renderer::GetInstance()->Update(dt);
	entityManagerSPL->pRenderer = Renderer::GetInstance();

}

void SplashScene::Render()
{
	
	return;
}

Engine::EngineCode SplashScene::Unload()
{
	EntityContainer::GetInstance()->FreeAll();
	Renderer::GetInstance()->CleanRenderer();
	return Engine::NothingBad;
}

Engine::EngineCode SplashScene::Exit()
{

	return Engine::NothingBad;
}

Scene* SplashSceneGetInstance(void)
{
	SplashSceneinstance = new SplashScene();
	return SplashSceneinstance;
}


void SplashScene::ImGuiInterg()
{
#ifdef _DEBUG
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Render();
#endif
}
