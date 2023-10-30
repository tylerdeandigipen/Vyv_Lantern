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
	void ReadTileMap(std::string filename, Renderer* pixel, bool IsNormal = false);
	int ReadInt(FILE* stream);
	float ReadFloat(FILE* stream);
	ImageBuffer* ReadPPM(const char* filename);
	static FileIO* GetInstance();
private:
	static FileIO* instance;
};

