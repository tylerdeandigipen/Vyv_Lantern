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

json FileIO::OpenJSON(std::string filename)
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
		exit(-1);
	}
	return jsonData;
}

void FileIO::ReadLight(Light& light, json& jsonData)
{
	light.Type = ReadLightEnum(jsonData["Type"]);

	if (jsonData["position"].is_object())
	{
		json position = jsonData["position"];
		light.position.x = position["x"];
		light.position.y = position["y"];
	}

	//light.leftAnglePos = jsonData["leftAnglePos"];
	//if (jsonData["leftAnglePos"].is_object())
	//{
	//	json leftAngle = jsonData["leftAnglePos"];
	//	light.leftAnglePos.x = leftAngle["x"];
	//	light.leftAnglePos.y = leftAngle["y"];
	//}

	//light.rightAnglePos = jsonData["rightAnglePos"];
	//if (jsonData["rightAnglePos"].is_object())
	//{
	//	json rightAngle = jsonData["rightAnglePos"];
	//	light.leftAnglePos.x = rightAngle["x"];
	//	light.leftAnglePos.y = rightAngle["y"];
	//}

	//light.color = jsonData["color"];
	if (jsonData["color"].is_object())
	{
		json color = jsonData["color"];
		light.color = { color["r"], color["g"], color["b"], color["a"] };
	}
	
	light.minAngle = jsonData["minAngle"];
	light.maxAngle = jsonData["maxAngle"];
	light.angle = jsonData["angle"];
	light.intensity = jsonData["intensity"];
	light.radius = jsonData["radius"];
	light.radialFalloff = jsonData["radialFalloff"];
	light.radialWeight = jsonData["radialWeight"];
	light.angularWeight = jsonData["angularWeight"];
	light.volumetricIntensity = jsonData["volumetricIntensity"];
	light.frustumWeight = jsonData["frustumWeight"];
}

void FileIO::ReadLight(std::string filename, Light& light)
{
	json jsonData = OpenJSON(filename);
	ReadLight(light, jsonData);
}

light_source_type FileIO::ReadLightEnum(std::string jsonString)
{
	std::string enumString = jsonString;
	if (enumString.compare("LightSourceType_Point") == 0)
		return LightSourceType_Point;
	if (enumString.compare("LightSourceType_Directional") == 0)
		return LightSourceType_Directional;
	if (enumString.compare("LightSourceType_EnumCount") == 0)
		return LightSourceType_EnumCount;

	return light_source_type::LightSourceType_Point;
}

FileIO* FileIO::GetInstance()
{
	return instance;
}

void FileIO::ReadTileMap(std::string filename, Renderer* pixel, bool IsNormal)
{
	json jsonData = OpenJSON(filename);
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
	for (int i = 0; i < spriteSheet->BufferSizeY / frameSize.y; ++i)
	{
		for (int j = 0; j < (spriteSheet->BufferSizeX / frameSize.x); ++j)
		{
			temp = new ImageBuffer{ frameSize.x, frameSize.y };
			spriteSheet->position.x = -(frameSize.x * j);
			spriteSheet->position.y = -(frameSize.y * i);
			temp->AddSprite(spriteSheet);
			if (IsNormal == false)
			{
				pixel->AddTileToTileset(temp);
			}
			else
			{
				pixel->AddNormalToNormalTileset(temp);
			}
		}
	}
	delete spriteSheet;
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