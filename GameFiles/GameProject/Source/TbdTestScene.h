//------------------------------------------------------------------------------
//
// File Name:	TbdTestScene.h
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
#include "EntityManager.h"

#include "LaserSystem.h"

class Scene;
struct emitter_id;

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
	std::unique_ptr<EntityManager> entityManagerTBD;
	emitter_id ControlledEmitter;

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
	laser_emitter* NewEmitter(void)
	{
		laser_emitter* Result = NULL;

		emitter_id ResultID = LaserSystem::GetInstance()->CreateEmitter();
		Result = LaserSystem::GetInstance()->GetEmitter(ResultID);

		return(Result);
	}
	reflector* NewReflector(void)
	{
		reflector* Result = NULL;

		reflector_id ResultID = LaserSystem::GetInstance()->CreateReflector();
		Result = LaserSystem::GetInstance()->GetReflector(ResultID);

		return(Result);
	}
};

Scene* TbdTestSceneGetInstance(void);
