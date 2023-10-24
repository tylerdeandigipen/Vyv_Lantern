#include "FileIO.h"
#include "Renderer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

FileIO* FileIO::instance = new FileIO;

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
	if (instance)
		delete instance;
}

FileIO* FileIO::GetInstance()
{
	return instance;
}

void FileIO::ReadTileMap(std::string filename, Renderer* pixel)
{
	json jsonData;
	std::fstream file(filename);
	if (file.is_open())
	{
		file >> jsonData;
		file.close();
	}
	else
	{
		perror("JSON file does not exist");
		return;
	}
	Vector2 frameSize;

	if (jsonData["FrameSize"].is_object())
	{
		json frame = jsonData["FrameSize"];
		frameSize.x = frame["X"];
		frameSize.y = frame["Y"];
	}
	ImageBuffer* spriteSheet = new ImageBuffer(jsonData["TilePPM"]);
	spriteSheet->position.y = 0;
	ImageBuffer* temp;
	for (int i = 0; i < spriteSheet->BufferSizeX / frameSize.x; ++i)
	{
		for (int j = 0; j < spriteSheet->BufferSizeY / frameSize.y; ++j)
		{
			temp = new ImageBuffer{ frameSize.x, frameSize.y };
			spriteSheet->position.x = -(frameSize.x * i);
			pixel->AddObject(spriteSheet);
		}
	}
}

int FileIO::ReadInt(FILE* stream)
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

float FileIO::ReadFloat(FILE* stream)
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
	/*FILE* fp;
	fopen_s(&fp, filename, "r");
	if (fp)
	{
		int tempX = 0;
		int tempY = 0;
		char hold = NULL;
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
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
				image->buffer[(i * image->BufferSizeX) + j] = { red, green, blue, 255 };
			}
		}
		return image;
	}*/
	return NULL;
}