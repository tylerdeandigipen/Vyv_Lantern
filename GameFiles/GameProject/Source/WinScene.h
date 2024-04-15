#pragma once

#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "EntityManager.h"

class Scene;

class WinScene : public Scene
{
public:

	WinScene();
	~WinScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
	std::unique_ptr<EntityManager> entityManagerTBD;

	void handleCheatCodes();

	void cheatFullbright();
	void cheatOnlyLights();
	void cheatNormalMap();
	void cheatWallHitboxes();
	void cheatScanlines();
	void cheatBlur();
	void cheatOnlyFog();
	void cheatFog();
	void ImGuiInterg();
	void ImGuiWindow();
};

Scene* WinSceneGetInstance(void);

