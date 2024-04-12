#pragma once

#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Entity.h"
#include "FileIO.h"
#include "EntityManager.h"
#include <SDL/SDL.h>
#include <glad/glad.h>

class Scene;



class SplashScene : public Scene
{
public:

	SplashScene();
	~SplashScene();

	void ImGuiInterg();
	void cheatScanlines();
	//void ImGuiWindow();
	void logoswich(bool check);

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
	float time;
	Entity* logo;
	bool logoflag;
	std::unique_ptr<EntityManager> entityManagerSPL;
	

	
};

Scene* SplashSceneGetInstance(void);