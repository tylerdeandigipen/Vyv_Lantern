//------------------------------------------------------------------------------
//
// File Name:	TestScene.cpp
// Author(s):	TayLee Young
// Purpose:		TEST SCENE PLEASE WORK
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "TestScene.h"
#include "Scene.h"
#include "PlatformSystem.h"
#include "Engine.h"
#include "SceneSystem.h"

//TestScene* instance = new TestScene();

//Scene* TestScene::GetInstance() { return &(instance->base); }

Engine::EngineCode TestScene::Load()
{
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Init()
{
	return Engine::NothingBad;
}

void TestScene::Update(float dt)
{
	//change this lmfao
	dt = 0;
}
void TestScene::Render()
{
	return;
}

Engine::EngineCode TestScene::Exit()
{
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Unload()
{
	return Engine::NothingBad;
}