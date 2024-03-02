#pragma once

#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"

class Scene;

class MenuScene : public Scene
{
public:

	MenuScene();
	~MenuScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
	void RenderButtons();
	void HandleButtonInput();

	bool IsMouseOverBackButton();
	bool IsMouseOverExitButton();
	bool IsMouseOverSettingsButton();

	void HandleBack();
	void HandleExit();
	void HandleSettings();
};

Scene* MenuSceneGetInstance(void);