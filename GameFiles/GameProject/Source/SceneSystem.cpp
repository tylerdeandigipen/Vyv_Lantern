// ------------------------------------------------------------------------------
//
// File Name:	SceneSystem.cpp
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		handles scenes (thumbs up)
//
// Copyright  2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include <cassert>
#include "SceneSystem.h"
#include "Renderer.h"
#include "PlatformSystem.h"
#include "Scene.h"
#include "Engine.h"
#include "TbdTestScene.h"
#include "Section1Final.h"
#include "TestScene.h"
#include "TBDTestScene.h"
#include "LevelCreatorScene.h"
#include "SplashScene.h"
#include "Inputs.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "MenuScene.h"
#include "Renderer.h"

// enums for different scene types
static bool Animating = false;
static bool FadeIn = false;
enum class SceneType
{
	SCENE_TBD_TEST = 0,
	SCENE_TEST = 1,
	SCENE_LEVELCREATOR = 2,
	SCENE_SPLASH = 3,
	SCENE_WIN = 4,
	SCENE_SECTION1,
	SCENE_LEVEL1,
	SCENE_LEVEL2,
	SCENE_LEVEL3,
	SCENE_LEVEL4,
	SCENE_MENU = 4,
	NULL_SCENE,

	// Add more scenes as needed
};

// singleton instance
std::unique_ptr<SceneSystem> SceneSystem::instance = nullptr;

Inputs* inputHandlerScene = Inputs::GetInstance();

Engine::EngineCode SceneSystem::Init()
{
	// make sure the default scene isn't null
	assert(DefaultSceneInstance != nullptr && "Default scene is NULL. Location: SceneSystem::Init()");

	//current scene instance set to default
	instance->SetScene(DefaultSceneInstance);
	return Engine::NothingBad;
}

static float tint = 1.0f;
void SceneSystem::Update(float dt)
{
	if (nextScene != nullptr)
	{
		ChangeScene();
		Animating = true;
	}

	// if there is an active scene and engine isn't paused
	if (activeScene && !Engine::GetInstance()->Paused())
	{
		// dt stuff!
		timer += dt;
		while (timer >= rate)
		{
			timer -= rate;
		}
	}
	else
	{
		dt = 0;
	}

	if (Animating)
	{
		if (!FadeIn)
		{
			tint -= .01;
			if (tint >= 0)
			{
				FadeIn = true;
				tint = 0.0f;
			}
		}
		else
		{
			tint += .01;
			if (tint >= 1)
			{
				tint = 1.0f;
				FadeIn = false;
				Animating = false;
			}
		}
		Renderer::GetInstance()->TintScreenBlack(tint);
	}
	assert(activeScene != nullptr && "Active scene is NULL. Location: SceneSystem::Update()");
	activeScene->Update(dt);
}

void SceneSystem::Render()
{
	// make sure scene exists
	assert(activeScene != nullptr && "Active scene is NULL. Location: SceneSystem::Render()");

	// render active scene
	activeScene->Render();
}

Engine::EngineCode SceneSystem::Close()
{
	if (activeScene)
	{
		// exit actions and unload resources
		activeScene->Exit();
		activeScene->Unload();
		activeScene = nullptr;
	}

	return Engine::NothingBad;
}

void SceneSystem::SetScene(Scene* scene)
{
	// next scene to be loaded, if any
	nextScene = scene;
}

void SceneSystem::RestartScene()
{
	// make sure scene exists
	assert(activeScene != nullptr);

	// sets scene to be active, restarting it
	SceneSystem::GetInstance()->SetScene(SceneSystem::GetInstance()->GetActiveScene());
}

float SceneSystem::GetRate()
{
	// return refresh rate
	return rate;
}

void SceneSystem::Exit()
{
	// close the window
	throw Engine::CloseWindow;
}

Scene* SceneSystem::GetActiveScene()
{
	return activeScene;
}

SceneSystem* SceneSystem::GetInstance()
{
	if (!instance)
	{
		instance.reset(new SceneSystem());
	}
	return instance.get();
}

// I CANNOT STRESS ENOUGH CHANGE DEFAULT SCENE INSTANCE ONCE WE HAVE A FEW SCENES GOING POR FAVOR
// PLEASEEEEEEEEEEEEEEEEEEE
// THIS IS FOR ME MOSTLY!!
// IM MAKING THIS PURPOSELY BIG SO I SEE IT MORE CLEARLY
// IGNORE THIS PRACTICALLY EVERYONE ELSE
// - taylee
SceneSystem::SceneSystem() : BaseSystem("SceneSystem"), DefaultSceneInstance(SplashSceneGetInstance()/*TbdTestSceneGetInstance() SplashSceneGetInstance()    MenuSceneGetInstance()*/),
activeScene(nullptr), nextScene(nullptr), timer(0), rate(0.01f), isRestarting(false)
{ }

SceneSystem::~SceneSystem()
{
}

void SceneSystem::ChangeScene()
{
	if (!nextScene)
	{
		assert(false && "nextScene is NULL! Location: SceneSystem::ChangeScene()");
		throw(Engine::AllScenesNull);
	}

	Renderer::GetInstance()->ResetMenuPages();

	if (!activeScene)
	{
		activeScene = nextScene;
		if (!activeScene && !nextScene)
		{
			activeScene = DefaultSceneInstance;
		}
		if (activeScene)
		{
			activeScene->Load();
		}
	}
	else
	{
		activeScene->Exit();

		// NOTE(thomas): Reset the pools for the active lasers and emitters once we change scenes
		LaserSystem::GetInstance()->HandleSceneChange();

		if (activeScene != nextScene || isRestarting)
		{
			activeScene->Unload();
			activeScene = nextScene;
			if (activeScene)
			{
				activeScene->Load();
			}
		}

		isRestarting = false;
	}

	if (!nextScene)
	{
		nextScene = DefaultSceneInstance;
	}

	nextScene = NULL;

	if (!activeScene)
	{
		assert(false && "activeScene is NULL! Location: SceneSystem::ChangeScene()");
		throw(Engine::AllScenesNull);
	}

	activeScene->Init();
}

bool CheckGameScenes()
{
	SceneSystem* sceneSystem = SceneSystem::GetInstance();
	SceneType activeSceneType = SceneType::NULL_SCENE;

	Scene* activeScene = sceneSystem->GetActiveScene();

	assert(activeScene != nullptr && "Active scene is NULL. Location: CheckGameScenes()");

	if (activeScene == TbdTestSceneGetInstance())
	{
		activeSceneType = SceneType::SCENE_TBD_TEST;
	}
	else if (activeScene == TestSceneGetInstance())
	{
		activeSceneType = SceneType::SCENE_TEST;
	}
	else if (activeScene == LevelCreatorSceneGetInstance())
	{
		activeSceneType = SceneType::SCENE_LEVELCREATOR;
	}
	else if (activeScene == SplashSceneGetInstance())
	{
		activeSceneType = SceneType::SCENE_SPLASH;
	}
	else if (activeScene == MenuSceneGetInstance())
	{
		activeSceneType = SceneType::SCENE_MENU;
	}
	else if (activeScene == Section1FinalGetInstance())
	{
		activeSceneType = SceneType::SCENE_SECTION1;
	}
	else if (activeScene == Level1GetInstance())
	{
		activeSceneType = SceneType::SCENE_LEVEL1;
	}
	else if (activeScene == Level1GetInstance())
	{
		activeSceneType = SceneType::SCENE_LEVEL1;
	}
	else if (activeScene == Level2GetInstance())
	{
		activeSceneType = SceneType::SCENE_LEVEL2;
	}
	else if (activeScene == Level3GetInstance())
	{
		activeSceneType = SceneType::SCENE_LEVEL3;
	}
	else if (activeScene == Level4GetInstance())
	{
		activeSceneType = SceneType::SCENE_LEVEL3;
	}

	assert(activeSceneType != SceneType::NULL_SCENE && "Active scene type is NULL. Location: CheckGameScenes()");

#ifdef _DEBUG
	if (inputHandlerScene->keyPressed(SDL_SCANCODE_1))
	{
		if (activeSceneType == SceneType::SCENE_TBD_TEST)
		{
			sceneSystem->RestartScene();
		}
		else
		{
			sceneSystem->SetScene(TbdTestSceneGetInstance());
		}
	}
	else if (inputHandlerScene->keyPressed(SDL_SCANCODE_2))
	{
		if (activeSceneType == SceneType::SCENE_TEST)
		{
			sceneSystem->RestartScene();
		}
		else
		{
			sceneSystem->SetScene(TestSceneGetInstance());
		}
	}
	else if (inputHandlerScene->keyPressed(SDL_SCANCODE_3))
	{
		if (activeSceneType == SceneType::SCENE_LEVELCREATOR)
		{
			sceneSystem->RestartScene();
		}
		else
		{
			sceneSystem->SetScene(LevelCreatorSceneGetInstance());
		}
	}
	else if (inputHandlerScene->keyPressed(SDL_SCANCODE_4))
	{
		if (activeSceneType == SceneType::SCENE_LEVELCREATOR)
		{
			sceneSystem->RestartScene();
		}
		else
		{
			sceneSystem->SetScene(SplashSceneGetInstance());
		}
	}
	else if (inputHandlerScene->keyPressed(SDL_SCANCODE_5))
	{
		if (activeSceneType == SceneType::SCENE_MENU)
		{
			sceneSystem->RestartScene();
		}
		else
		{
			sceneSystem->SetScene(MenuSceneGetInstance());
		}
	}
	else if (inputHandlerScene->keyPressed(SDL_SCANCODE_7))
	{
		SceneSystem::GetInstance()->SetScene(Level4GetInstance());
	}
	else
	{
		return false;
	}
#endif
	return false;
}

bool CheckRestart()
{
#ifdef _DEBUG
	if (inputHandlerScene->keyPressed(SDL_SCANCODE_R))
	{
		SceneSystem::GetInstance()->RestartScene();
		return true;
	}
#endif
	return false;
}

bool CheckRestart(const char* filename)
{
#ifdef _DEBUG
	if (inputHandlerScene->keyPressed(SDL_SCANCODE_R))
	{
		SceneSystem::GetInstance()->RestartScene();
		return true;
	}
#endif
	return false;
}