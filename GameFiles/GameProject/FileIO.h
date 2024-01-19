//------------------------------------------------------------------------------
//
// File Name:	FileIO.h
// Author(s):	Michael Howard
// Purpose:		Reading of files from JSON.
//
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "Light.h"
#include "ImageBuffer.h"

class Renderer;

class FileIO
{
public:
	FileIO();
	~FileIO();
	json OpenJSON(std::string filename);
	void ReadLight(Light& light, json& jsonData);
	void ReadLight(std::string filename, Light& light);
	light_source_type ReadLightEnum(std::string); //pass json["enumpart"]
	// 0 = regular tileset, 1 = normal tileset, 2 = shadow tileset
	void ReadTileSet(std::string filename, Renderer* pixel, int tileSetType = 0);
	int ReadInt(FILE* stream);
	float ReadFloat(FILE* stream);

	Entity* ReadEntity(json jsonData);

	ImageBuffer* ReadPPM(const char* filename);

	void ExportTileMap(std::string name);

	int** ReadTiledMap(std::string filename, int** tilemap);
	int** ReadTylerTileMap(std::string filename, int** tilemap);

	int** ReadTiledMap(json jsonData);
	int** ReadTylerTileMap(json jsonData);

	static FileIO* GetInstance();
private:
	static FileIO* instance;
};

