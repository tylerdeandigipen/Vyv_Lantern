//------------------------------------------------------------------------------
//
// File Name:	LevelCreatorScene.h
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
#include "Inputs.h"


class Scene;
struct EntityProperties;

int ApplyProperties(EntityProperties properties);

class LevelCreatorScene : public Scene
{
public:
	
	LevelCreatorScene();

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
	void ToolHandler();
	Vector2 PlaceTile(Vector2 tilePos);
	Vector2 PlaceObject(Vector2 objectPos);
	void ToolPan(Inputs* inputHandler, Vector2 CursourP);
	void ToolBrush(Inputs* inputHandler, Vector2 CursourP);
	void ToolEyedroper(Inputs* inputHandler, Vector2 CursourP);
	void ToolCenter(Inputs* inputHandler);
	void ToolSquareFill(Inputs* inputHandler, Vector2 CursourP);
	int CreateCircleEntity();
	int CreateDoorEntity();
	int CreateMirrorEntity();

	char myTextBuffer[256]{};

	enum ObjType
	{
		LIGHT_EMITTER,
		COUNT
	};
};

Scene* LevelCreatorSceneGetInstance(void);
