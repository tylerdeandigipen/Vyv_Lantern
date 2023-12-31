//------------------------------------------------------------------------------
//
// File Name:	TbdTestScene.h
// Author(s):	TayLee Young, Louis Wang, Michael Howard, Tyler Dean
// Purpose:		Test scene for our game
//
// Copyright  � 2023 DigiPen (USA) Corporation.
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

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
	bool winState = false;
private:
	void ImGuiInterg();
	void ImGuiWindow();
	void handleCheatCodes();

	void cheatFullbright();
	void cheatOnlyLights();
	void cheatNormalMap();
	void cheatWallHitboxes();
	void cheatScanlines();
	void cheatBlur();
	void cheatOnlyFog();
	void cheatFog();
};

Scene* TbdTestSceneGetInstance(void);
