#pragma once
#include "stdio.h"
typedef class ImageBuffer;
class FileIO
{
public:
	FileIO();
	~FileIO();
	int ReadInt(FILE* stream);
	float ReadFloat(FILE* stream);
	ImageBuffer* ReadPPM(const char* filename);
};

