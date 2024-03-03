//------------------------------------------------------------------------------
//
// File Name:	SplashScene.cpp
// Author(s):	
// Purpose:		Change this to whatever you need
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
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

SplashScene::SplashScene() : Scene("Splash")
{
	time = 3.0f;
	logo = nullptr;	
}

SplashScene::~SplashScene()
{

}

Engine::EngineCode SplashScene::Init()
{
	return Engine::NothingBad;
}

Engine::EngineCode SplashScene::Load()
{
	Renderer::GetInstance()->isFullbright = true;
	//TODO make a LOGO sprite

	LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/Splash/Splash.json");

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
		
	}
	else
	{
		time -= dt;
	}

	EntityContainer::GetInstance()->UpdateAll(dt);
	Renderer::GetInstance()->Update(dt);

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