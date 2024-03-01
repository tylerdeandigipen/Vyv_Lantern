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

Scene* SplashSceneinstance = NULL; //SORRY MIKEY L MEYERS!!!!!!!!!!!!!!!!!!!!!!

SplashScene::SplashScene() : Scene("Splash")
{

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
	return Engine::NothingBad;
}

void SplashScene::Update(float dt)
{
	dt = dt;
}

void SplashScene::Render()
{
	return;
}

Engine::EngineCode SplashScene::Unload()
{
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