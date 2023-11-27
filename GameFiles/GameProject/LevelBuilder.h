#pragma once

#include "stdafx.h"
#include "BaseSystem.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class EntityContainer;
class Renderer;

class LevelBuilder : public BaseSystem
{
public:
	LevelBuilder();
	~LevelBuilder();
	void Read(json &jsonData);
	Engine::EngineCode Init();
	void Update(float dt);
	void LevelUpdate(float dt);

	void Render();
	Engine::EngineCode Close();
	
	static LevelBuilder* GetInstance();
	void LoadLevel(std::string filename);
	void ReLoadLevel();
	void FreeLevel();
	static void SetWinState(bool state);
	static bool GetWinState();
	void SetTileMap(int** tiles);
	int** GetTileMap();
	int CountEntities();
	void SetWalls(int* walls);
	int* GetWalls();
	void SetX(int size);
	void SetY(int siye);
	int GetX();
	int GetY();
	const EntityContainer* GetContainer();

private:
	void AddTileSets();
	static LevelBuilder* instance;
	static bool WinState;
	EntityContainer* entity_container;
	json jsonData;
	int** TileMap;
	int* Walls;
	int SizeX;
	int SizeY;
};

