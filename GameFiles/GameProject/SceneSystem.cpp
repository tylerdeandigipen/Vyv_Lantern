// ------------------------------------------------------------------------------
//
// File Name:	SceneSystem.cpp
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		handles scenes (thumbs up)
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include <cassert>
#include "SceneSystem.h"
#include "PlatformSystem.h"
#include "Scene.h"
#include "Engine.h"
#include "TbdTestScene.h"
#include "TestScene.h"
#include "TBDTestScene.h"

#include "Inputs.h"

enum SceneType 
{
	SCENE_TBD_TEST,
	SCENE_TEST,
	NULL_SCENE,
	// Add more scenes as needed
};

SceneSystem * SceneSystem::instance = new SceneSystem();

Inputs* inputHandlerScene = Inputs::GetInstance();

Engine::EngineCode SceneSystem::Init()
{
	assert(DefaultSceneInstance != NULL);
	instance->SetScene(DefaultSceneInstance);
	return Engine::NothingBad;
}

void SceneSystem::Update(float dt)
{
	if (nextScene != NULL)
	{
		ChangeScene();
	}

	if (activeScene && !Engine::GetInstance()->Paused())
	{
		activeScene->Update(dt);

		timer += dt;
		while (timer >= rate)
		{
			timer -= rate;
		}
	}
}

void SceneSystem::Render()
{
	assert(activeScene != NULL);
	activeScene->Render();
}

Engine::EngineCode SceneSystem::Close()
{
	if (activeScene)
	{
		activeScene->Exit();
		activeScene->Unload();
		activeScene = NULL;
	}

	return Engine::NothingBad;
}

void SceneSystem::SetScene(Scene* scene)
{
	nextScene = scene;
}

void SceneSystem::RestartScene()
{
	assert(activeScene != NULL);
	SetScene(activeScene);
}

float SceneSystem::GetRate()
{
	return rate;
}

void SceneSystem::Exit()
{
	throw Engine::CloseWindow;
}

Scene* SceneSystem::GetActiveScene()
{
	return activeScene;
}

SceneSystem* SceneSystem::GetInstance()
{
	return instance;
}

// I CANNOT STRESS ENOUGH CHANGE DEFAULT SCENE INSTANCE ONCE WE HAVE A FEW SCENES GOING POR FAVOR
// PLEASEEEEEEEEEEEEEEEEEEE
// THIS IS FOR ME MOSTLY!!
// IM MAKING THIS PURPOSELY BIG SO I SEE IT MORE CLEARLY
// IGNORE THIS PRACTICALLY EVERYONE ELSE
// - taylee
SceneSystem::SceneSystem() : BaseSystem("SceneSystem"), DefaultSceneInstance(TbdTestSceneGetInstance()),
activeScene(NULL), nextScene(NULL), timer(0), rate(0.01f), isRestarting(false)
{ }

SceneSystem::~SceneSystem()
{
	if (instance != NULL)
	{
		delete instance;
	}
}

void SceneSystem::ChangeScene()
{
	if (!nextScene)
	{
		assert(false && "nextScene is NULL!");
		throw(Engine::AllScenesNull);
	}

	if (!activeScene)
	{
		activeScene = nextScene;
		if (!activeScene && !nextScene)
			activeScene = DefaultSceneInstance;
		activeScene->Load();
	}
	else
	{
		activeScene->Exit();

		if (activeScene != nextScene || isRestarting)
		{
			activeScene->Unload();
			activeScene = nextScene;
			activeScene->Load();
		}

		isRestarting = false;

	}

	nextScene = NULL;
	activeScene->Init();
}

bool CheckGameScenes()
{
	SceneSystem* sceneSystem = SceneSystem::GetInstance();
	SceneType activeSceneType = NULL_SCENE;

	Scene* activeScene = sceneSystem->GetActiveScene();

	if (activeScene == TbdTestSceneGetInstance()) {
		activeSceneType = SCENE_TBD_TEST;
	}
	else if (activeScene == TestSceneGetInstance()) {
		activeSceneType = SCENE_TEST;
	}

	if (inputHandlerScene->keyPressed(SDL_SCANCODE_1) && activeSceneType != SCENE_TBD_TEST)
	{
		sceneSystem->SetScene(TbdTestSceneGetInstance());
	}
	else if (inputHandlerScene->keyPressed(SDL_SCANCODE_2) && activeSceneType != SCENE_TEST)
	{
		sceneSystem->SetScene(TestSceneGetInstance());
	}
	else
	{
		return false;
	}

	return true;
}


bool CheckRestart()
{
	if (inputHandlerScene->keyPressed(SDL_SCANCODE_R) && dynamic_cast<TestScene*>(SceneSystem::GetInstance()->GetActiveScene()))
	{
		SceneSystem::GetInstance()->RestartScene();
		return true;
	}
	return false;
}