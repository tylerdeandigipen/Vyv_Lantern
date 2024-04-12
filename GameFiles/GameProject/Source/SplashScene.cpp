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
#include "MenuScene.h"
#include "Inputs.h"
#include "DebugNew.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Scene* SplashSceneinstance = NULL; //SORRY MIKEY L MEYERS!!!!!!!!!!!!!!!!!!!!!!

SDL_Window* SplashWindow;

SDL_GLContext SplashGlContext;

Inputs* inputs = nullptr;

SplashScene::SplashScene() : Scene("Splash")
{
	time = 6.0f;
	logo = nullptr;	
	logoflag = true;
	entityManagerSPL = std::make_unique<EntityManager>();
	if (!inputs)
	{
		inputs = Inputs::GetInstance();
	}
}

SplashScene::~SplashScene()
{
}


Engine::EngineCode SplashScene::Init()
{
	Renderer::GetInstance()->isFullbright = true;
	//TODO make a LOGO sprite
		// Create SDL Window
	cheatScanlines();

	json ignore{ {"isAnimated", false} };
	logo = new Entity("Object", "./Assets/PPM/logo.ppm", ignore);

	logo->CreateImage("./Assets/PPM/logo.ppm");
	

	Transform* LogoPOS = new Transform;
	LogoPOS->SetTranslation(gfxVector2{ 50,50 });

	logo->Add(LogoPOS);
	logo->AddToRenderer(Renderer::GetInstance(), "");



	SplashWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	Renderer::GetInstance()->window = SplashWindow;

	LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/Splash2/Splash2.json");


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

	if (!inputs)
	{
		inputs = Inputs::GetInstance();
	}
	
	if (time <= 0.0f || inputs->keyPressed(SDL_SCANCODE_E) || inputs->keyPressed(SDL_SCANCODE_ESCAPE) || inputs->keyPressed(SDL_SCANCODE_SPACE) || inputs->keyPressed(SDL_SCANCODE_RETURN))
	{
		//TODO get the menu scene.
		//Renderer::GetInstance()->isFullbright = false;
		SceneSystem::GetInstance()->SetScene(MenuSceneGetInstance());
		ImGuiInterg();

	}
	else if (time <= 3.0f)
	{
		logoswich(false);
		logo->Update(dt);
		time -= dt;
	}
	else
	{
		logo->Update(dt);
		time -= dt;
	}

	EntityContainer::GetInstance()->UpdateAll(dt);
	Renderer::GetInstance()->Update(dt);
	entityManagerSPL->pRenderer = Renderer::GetInstance();

}

void SplashScene::logoswich(bool check)
{
	if (logoflag == check)
	{
		return;
	}
	else
	{
		logo->SetImage("./Assets/PPM/flogo.ppm");
		logoflag = check;
	}
}


void SplashScene::cheatScanlines()
{
	if (Renderer::GetInstance()->doScanLines == false)
		Renderer::GetInstance()->doScanLines = true;
	else
		Renderer::GetInstance()->doScanLines = false;
}

void SplashScene::Render()
{
	
	return;
}

Engine::EngineCode SplashScene::Unload()
{
	delete logo->GetComponent<Transform>();
	delete logo;
	logo = nullptr;
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
	//SplashSceneinstance = new SplashScene();
	static Scene* SplashSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!SplashSceneinstance) {
		SplashSceneinstance = new SplashScene();
	}
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
