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
	void ReadTileSet(std::string filename, Renderer* pixel, bool IsNormal = false);
	int ReadInt(FILE* stream);
	float ReadFloat(FILE* stream);
	ImageBuffer* ReadPPM(const char* filename);

	int** ReadTiledMap(std::string filename, int** tilemap);
	int** ReadTylerTileMap(std::string filename, int** tilemap);

	int** ReadTiledMap(json jsonData);
	int** ReadTylerTileMap(json jsonData);

	static FileIO* GetInstance();
private:
	static FileIO* instance;
};

