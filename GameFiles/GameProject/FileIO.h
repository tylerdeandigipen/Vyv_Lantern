#pragma once
#include "stdafx.h"
#include "ImageBuffer.h"

class Renderer;

class FileIO
{
public:
	FileIO();
	~FileIO();
	void ReadTileMap(std::string filename, Renderer* pixel);
	int ReadInt(FILE* stream);
	float ReadFloat(FILE* stream);
	ImageBuffer* ReadPPM(const char* filename);
	static FileIO* GetInstance();
private:
	static FileIO* instance;
};

