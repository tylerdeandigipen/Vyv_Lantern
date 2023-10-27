#pragma once

#include "stdafx.h"
#include "BaseSystem.h"
#include "EntityContainer.h"
#include "Renderer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class LevelBuilder : public BaseSystem
{
public:
	LevelBuilder();
	~LevelBuilder();
	void Read(json const& jsonData, Renderer* pixel);
	Engine::EngineCode Init();
	void Update(float dt);
	void LevelUpdate(float dt);

	void Render();
	Engine::EngineCode Close();

	static LevelBuilder* GetInstance();
	void LoadLevel(Renderer* pixel, std::string filename);
	void ReLoadLevel();
	void FreeLevel();
	int** GetTileMap();
	int GetX();

	int GetY();
private:
	static LevelBuilder* instance;
	EntityContainer* entity_container;
	json jsonData;
	int** TileMap;
	int SizeX;
	int SizeY;
};

