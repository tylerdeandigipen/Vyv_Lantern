//------------------------------------------------------------------------------
//
// File Name:	SceneSystem.h
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		handles scenes (thumbs up)
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#include "BaseSystem.h"
#include <memory>

class Scene;

class SceneSystem : public BaseSystem
{
public:
	~SceneSystem();
	Engine::EngineCode Init();
	Engine::EngineCode Close();
	void Update(float dt);
	void Render();
	void Exit();
	static void RestartScene();
	void SetScene(Scene* scene);
	Scene* GetActiveScene();
	static SceneSystem* GetInstance();

	float GetRate();

private:
	SceneSystem();

	// prohibits copying and conversion! - taylee
	SceneSystem(SceneSystem const&) = delete;
	SceneSystem& operator=(SceneSystem const&) = delete;

	void ChangeScene();

	static std::unique_ptr<SceneSystem> instance;
	Scene* DefaultSceneInstance;

	Scene* activeScene;
	Scene* nextScene;
	float timer;
	float rate;
	bool isRestarting;
};

bool CheckGameScenes();
bool CheckRestart();