//------------------------------------------------------------------------------
//
// File Name:	TestScene.h
// Author(s):	TayLee Young
// Purpose:		PLEASE
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"

class Scene;

class TestScene
{
public:
	Scene base;

	Engine::EngineCode Load(void);
	Engine::EngineCode Init(void);
	Engine::EngineCode Exit(void);
	Engine::EngineCode Unload(void);
	void Update(float dt);
	void Render(void);

	
	TestScene(Scene _base) : base(_base)
	{
	}

	Scene* GetInstance(void);
private:
	static TestScene* instance;
	TestScene();
	~TestScene() = delete;
};

