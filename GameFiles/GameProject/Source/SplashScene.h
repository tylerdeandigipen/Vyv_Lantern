#pragma once

#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Entity.h"
#include "FileIO.h"
#include "EntityManager.h"

class Scene;

class SplashScene : public Scene
{
public:

	SplashScene();
	~SplashScene();

	void ImGuiInterg();
	//void ImGuiWindow();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
	float time;
	Entity* logo;
	std::unique_ptr<EntityManager> entityManagerSPL;

	
};

Scene* SplashSceneGetInstance(void);