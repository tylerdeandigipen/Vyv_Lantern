#include "FileIO.h"

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

int ReadInt(FILE* stream)
{
	int return_val = 0;
	if (stream)
	{
		fscanf_s(stream, "%d", &return_val);
		return return_val;
	}
	else
	{
		return 0;
	}
}

float ReadFloat(FILE* stream)
{
	float return_val = 0.0f;
	if (stream)
	{
		fscanf_s(stream, "%f", &return_val);
		return return_val;
	}
	else
	{
		return 0.0f;
	}
}

ImageBuffer* ReadPPM(const char* filename)
{
	FILE* fp;
	fopen_s(&fp, filename, "r");
	if (fp)
	{
		int tempX = 0;
		int tempY = 0;
		char hold = NULL;
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
		fscanf_s(fp, "%c", &hold);

		fscanf_s(fp, "%c", &hold);
		fscanf_s(fp, "%c", &hold);
		fscanf_s(fp, "%d", &tempX);
		fscanf_s(fp, "%d", &tempY);
		ImageBuffer* image = new ImageBuffer(tempX, tempY);
		for (int i = 0; i < image->BufferSizeX; ++i)
		{
			for (int j = 0; i < image->BufferSizeY; ++j)
			{
				fscanf_s(fp, "%f", &red);
				fscanf_s(fp, "%f", &green);
				fscanf_s(fp, "%f", &blue);
				image->buffer[i][j] = { red, green, blue, 255 };
			}
		}
		return image;
	}
	return NULL;
}