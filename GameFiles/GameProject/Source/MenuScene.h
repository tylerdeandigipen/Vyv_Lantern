//------------------------------------------------------------------------------
//
// File Name:	MenuScene.h
// Author(s):	TayLee Young, Louis Wang, Michael Howard, Tyler Dean
// Purpose:		Test scene for our game
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

#include "LaserSystem.h"

class Scene;
struct emitter_id;

class MenuScene : public Scene
{
public:

	MenuScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
	bool winState = false;
	bool IsMouseOverBeginingButton();
	bool IsMouseOverExitButton();
	bool IsMouseOverCreditButton();
	bool IsMouseOverOptionButton();

	void HandleBegin();
	void HandleExit();
	void HandleCredit();
	void HandleOption();


	void ImGuiInterg();
	void ImGuiWindow();


	void cheatScanlines();

private:
	emitter_id ControlledEmitter;


};

Scene* MenuSceneGetInstance(void);
