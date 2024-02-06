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
#include "stdafx.h"
#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "AudioEngine.h"
#include "Collision.h"
#include "Inputs.h"
#include "EntityContainer.h"
#include "Transform.h"

#include "Entity.h"
#include "TextEditor.h"

#include <map>

class EntityManager;
class Scene;
Scene* LevelCreatorSceneGetInstance(void);


struct EntityProperties
{
	int translation[2] = { 0 };
	float rotation;
	// bool isCollidable;
	bool isPicked = false;
	bool isEditable = false;
};

class LevelCreatorScene : public Scene
{
	friend class EntityManager;
public:
	
	LevelCreatorScene();
	~LevelCreatorScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
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
	int CreateCircleEntity();
	int CreateDoorEntity();
	int CreateMirrorEntity();
	void AddToFile(std::string nametoadd, Entity* entity);
	void ExportScene(std::string);
	
	// member variables
	bool winState = false;
	// have a current list as a default so it can just be overwritten 
	std::string currentGameObjectsList = "./Data/GameObjects/ObjectListLevelBuilder.json";
	std::string listToExport;
	json gameObjects;
	std::vector<Entity*> tempEntities;
	std::unordered_map<std::string, std::function<void (Entity*)>> AddFunc;
	std::string currentTileMapList;
	char myTextBuffer[256]{};
	char LoadBuffer[256]{};

	enum ObjType
	{
		LIGHT_EMITTER,
		COUNT
	};
};

/* fucntion for adding funcitons to map */

class EntityManager
{
public:
	void EditEntity(Vector2 mousePos);
	void ShowEntityInfo();
	int ApplyProperties();
	bool InitializeProperties(std::string file_path);
	void EditText();
	void EntityPicker();
	void SetMousePos(Vector2 mousePos);

	bool IsEntityPicked(const std::string& key) const;

	TextEditor editor;
	std::unordered_map<std::string, EntityProperties> properties;
	std::string fileToEdit = "Data/GameObjects/Circle.json";
	std::ifstream file;
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::CPlusPlus();
	Renderer* pRenderer;
	Vector2 mousePos_;
	Inputs* pInput;
};

