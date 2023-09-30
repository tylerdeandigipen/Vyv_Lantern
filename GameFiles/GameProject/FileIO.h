#pragma once
#include "stdio.h"
#include "ImageBuffer.h"

class FileIO
{
public:
	FileIO();
	~FileIO();
	int ReadInt(FILE* stream);
	float ReadFloat(FILE* stream);
	ImageBuffer* ReadPPM(const char* filename);
};

