#include "StubScene.h"
#include "Scene.h"
#include "SceneSystem.h"
#include <SDL/SDL.h>

Scene* instance = NULL; //SORRY MIKEY L MEYERS!!!!!!!!!!!!!!!!!!!!!!

StubScene::StubScene() : Scene("Stub")
{

}

StubScene::~StubScene()
{

}

Engine::EngineCode StubScene::Init()
{
	return Engine::NothingBad;
}

Engine::EngineCode StubScene::Load()
{
	return Engine::NothingBad;
}

void StubScene::Update(float dt)
{
	dt = dt;
}

void StubScene::Render()
{
	return;
}

Engine::EngineCode StubScene::Unload()
{
	return Engine::NothingBad;
}

Engine::EngineCode StubScene::Exit()
{
	return Engine::NothingBad;
}

Scene* StubSceneGetInstance(void)
{
	instance = new StubScene();
	return instance;
}