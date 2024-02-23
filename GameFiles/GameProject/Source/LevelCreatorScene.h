//------------------------------------------------------------------------------
//
// File Name:	LevelCreatorScene.h
// Author(s):	TayLee Young, Louis Wang, Michael Howard, Tyler Dean
// Purpose:		Test scene for our game
//
// Copyright  ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#include "EntityManager.h"

class Scene;
Scene* LevelCreatorSceneGetInstance(void);

class LevelCreatorScene : public Scene
{
	friend class EntityManager;
	friend class Entity; // do not judge me won
public:
	
	LevelCreatorScene();
	~LevelCreatorScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
	std::vector<Entity*> tempEntities;
	std::string exportFolder;
private:
	bool playerExists;
	void ImGuiInterg();
	void ImGuiWindow();

	Vector2 PlaceTile(Vector2 tilePos);
	Vector2 PlaceObject(Vector2 objectPos);

	void ToolHandler();
	void ToolPan(Inputs* inputHandler, Vector2 CursourP);
	void ToolBrush(Inputs* inputHandler, Vector2 CursourP);
	void ToolEyedroper(Inputs* inputHandler, Vector2 CursourP);
	void ToolCenter(Inputs* inputHandler);
	void ToolSquareFill(Inputs* inputHandler, Vector2 CursourP);

	static void AddCircleEntity(Entity* entity);
	static void AddDoorEntity(Entity* entity);
	static void AddMirrorEntity(Entity* entity);
	static void AddEmitterEntity(Entity* entity);
	static void AddRecieverEntity(Entity* entity);


	int CreatePlayerEntity();
	int CreateCircleEntity();
	int CreateDoorEntity();
	int CreateMirrorEntity();
	int CreateEmitterEntity();
	int CreateRecieverEntity();

	void AddToFile(std::string nametoadd, Entity* entity);
	void ExportScene(std::string);

	//int CreateEntity(const std::string& entityType);
	// assetFilePath is which asset do you want the thing to use. 
	// ex:
	// circle = "Circle_2x2";
	// door = "Door_Closed"
	// mirror = "Mirror"
	//void AddEntity(Entity* entity, const std::string& entityType, const std::string& assetFilePath);
	
	// member variables
	bool winState = false;
	// have a current list as a default so it can just be overwritten 
	std::string currentGameObjectsList = "./Data/GameObjects/ObjectListLevelBuilder.json";
	std::string listToExport;
	std::string sceneName;
	json gameObjects;
	std::unordered_map<std::string, std::function<void (Entity*)>> AddFunc;
	std::string currentTileMapList;
	char myTextBuffer[256]{};
	char LoadBuffer[256]{};
	std::unique_ptr<EntityManager> entityManager;

	enum ObjType
	{
		LIGHT_EMITTER,
		COUNT
	};
};
