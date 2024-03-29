//------------------------------------------------------------------------------
//
// File Name:	LevelBuilder.cpp
// Author(s):	Michael Howard
// Purpose:		Sets up the tilemap and game objects for each level
//
// Copyright  ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "LevelBuilder.h"
#include "Renderer.h"
#include "EntityContainer.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Logging.h"
#include "FileIO.h"
#include "Transform.h"

bool LevelBuilder::WinState = false;

bool LevelBuilder::door = false;

std::unique_ptr<LevelBuilder> LevelBuilder::instance = nullptr;

LevelBuilder::LevelBuilder() : entity_container(EntityContainer::GetInstance()), jsonData(), TileMap(NULL), Walls(NULL), SizeX(0), SizeY(0)
{
}

LevelBuilder::~LevelBuilder()
{
	if (TileMap)
	{
		for (int i = 0; i < SizeX; ++i)
		{
			delete[] TileMap[i];
		}
		delete[] TileMap;
	}
	if (Walls)
	{
		delete[] Walls;
	}
}

Engine::EngineCode LevelBuilder::Init()
{
	return Engine::NothingBad;
}

void LevelBuilder::LoadTileMap(std::string filename)
{
	Renderer* pixel = Renderer::GetInstance();
	std::ifstream file(filename);
	if (file.is_open())
	{
		Logging::GetInstance("debugLog.log").LogLine("Opened %s", filename.c_str());
		file >> jsonData;
		file.close();
	}
	else
	{
		Logging::GetInstance("debugLog.log").LogLine("Could not open %s", filename.c_str());

		//perror("JSON file does not exist");
	}

	AddTileSets();
	if (jsonData["TylerTileData"].is_object())
	{
		TileMap = FileIO::GetInstance()->ReadTylerTileMap(jsonData);
		pixel->tileMapSize.x = static_cast<float>(SizeX);
		pixel->tileMapSize.y = static_cast<float>(SizeY);
		pixel->MakeTileMap(TileMap);
	}
	else if (jsonData["TiledData"].is_object())
	{
		TileMap = FileIO::GetInstance()->ReadTiledMap(jsonData);
		pixel->tileMapSize.x = static_cast<float>(SizeX);
		pixel->tileMapSize.y = static_cast<float>(SizeY);
		pixel->MakeTileMap(TileMap);
	}

	if (jsonData["GameObjectList"].is_object())
	{
		json objectList = jsonData["GameObjectList"];
		EntityContainer::GetInstance()->ReadEntities(objectList["GameObjectFile"]);
	}

	//pixel->ResizeBuffers();
}

void LevelBuilder::Update(float dt)
{
	dt = dt;
}

void LevelBuilder::Render()
{
}

Engine::EngineCode LevelBuilder::Close()
{
	return Engine::NothingBad;
}

LevelBuilder* LevelBuilder::GetInstance()
{
	if (!instance)
	{
		instance.reset(new LevelBuilder());
	}
	return instance.get();
}

void LevelBuilder::ReLoadLevel()
{
}

void LevelBuilder::FreeLevel()
{
}

void LevelBuilder::SetWinState(bool state)
{
	WinState = state;
}

bool LevelBuilder::GetWinState()
{
	return WinState;
}

// Function to check if the win state is set
bool LevelBuilder::IsWinStateSet() {
	return WinState;
}

void LevelBuilder::setDoor(bool closeOpen)
{
	door = closeOpen;
}

bool LevelBuilder::getDoor()
{
	return door;
}

int** LevelBuilder::GetTileMap()
{
	return TileMap;
}

void LevelBuilder::SetWalls(int* walls)
{
	Walls = walls;
}

int* LevelBuilder::GetWalls()
{
	if (Walls)
	{
		return Walls;
	}
	else
	{
		return NULL;
	}
}

int LevelBuilder::GetX()
{
	return SizeX;
}

int LevelBuilder::GetY()
{
	return SizeY;
}

void LevelBuilder::SetX(int size)
{
	SizeX = size;
}

void LevelBuilder::SetY(int size)
{
	SizeY = size;
}

void LevelBuilder::AddTileSets()
{
	Renderer* pixel = Renderer::GetInstance();
	FileIO::GetInstance()->ReadTileSet("./Data/TileMapSprites.json", pixel);
	FileIO::GetInstance()->ReadTileSet("./Data/TileMapNormals.json", pixel, 1);
	FileIO::GetInstance()->ReadTileSet("./Data/TileMapShadows.json", pixel, 2);
}