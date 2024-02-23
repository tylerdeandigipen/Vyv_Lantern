//------------------------------------------------------------------------------
//
// File Name:	LevelBuilder.h
// Author(s):	Michael Howard
// Purpose:		Function declarations for building of the levels
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
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

	void Render();
	Engine::EngineCode Close();
	
	static LevelBuilder* GetInstance();
	void LoadTileMap(std::string filename);
	void ReLoadLevel();
	void FreeLevel();
	static void SetWinState(bool state);
	static bool GetWinState();
	static bool IsWinStateSet();
	void SetTileMap(int** tiles);
	int** GetTileMap();
	void SetWalls(int* walls);
	int* GetWalls();
	void SetX(int size);
	void SetY(int siye);
	int GetX();
	int GetY();

	static void setDoor(bool door);
	static bool getDoor();

private:
	void AddTileSets();
	static LevelBuilder* instance;
	static bool WinState;
	static bool door;
	EntityContainer* entity_container;
	json jsonData;
	int** TileMap;
	int* Walls;
	int SizeX;
	int SizeY;
};

