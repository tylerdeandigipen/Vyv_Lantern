//------------------------------------------------------------------------------
//
// File Name:	TestScene.h
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
#include "Renderer.h"
#include "AudioEngine.h"
#include "LaserSystem.h"
#include "Collision.h"

class Scene;
class TestScene : public Scene
{
public:
	
	TestScene();
	~TestScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
	void ImGuiInterg();
	void ImGuiWindow();
	
	emitter_id ControlledEmitter;


	laser_emitter *NewEmitter(void)
	{
		laser_emitter* Result = NULL;

		emitter_id ResultID = LaserSystem::GetInstance()->CreateEmitter();
		Result = LaserSystem::GetInstance()->GetEmitter(ResultID);

		return(Result);
	}

	reflector *NewReflector(void)
	{
		reflector* Result = NULL;

		reflector_id ResultID = LaserSystem::GetInstance()->CreateReflector();
		Result = LaserSystem::GetInstance()->GetReflector(ResultID);

		return(Result);
	}
};

Scene* TestSceneGetInstance(void);
