//------------------------------------------------------------------------------
//
// File Name:	TestScene.h
// Author(s):	TayLee Young
// Purpose:		PLEASE
//
// Copyright  � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "AudioEngine.h"
#include "LaserSystem.h"
#include "Collision.h"

class Scene;
class TestScene : public Scene
{
public:
	
	TestScene();
	~TestScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
	void InitImGui();
	void ImGuiInterg();
	void ImGuiWindow();
	void ImGuiExit();
    emitter_id ControlledEmitter;
};

Scene* TestSceneGetInstance(void);
