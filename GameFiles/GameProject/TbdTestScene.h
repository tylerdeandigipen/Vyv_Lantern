//------------------------------------------------------------------------------
//
// File Name:	TbdTestScene.h
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
#include "AudioEngine.h"
#include "Collision.h"

class Scene;

class TbdTestScene : public Scene
{
public:
	
	TbdTestScene();
	~TbdTestScene();

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
};

Scene* TbdTestSceneGetInstance(void);
