#pragma once

#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"

class Scene;

class StubScene : public Scene
{
public:

	StubScene();
	~StubScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
};

Scene* StubSceneGetInstance(void);