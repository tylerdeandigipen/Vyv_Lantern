//------------------------------------------------------------------------------
//
// File Name:	FileIO.cpp
// Author(s):	Michael Howard
// Purpose:		Reading JSON data
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "FileIO.h"
#include "ImageBuffer.h"
#include "Light.h"
#include "LevelBuilder.h"
#include "Renderer.h"
#include "Logging.h"


FileIO* FileIO::instance = new FileIO;

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
	if (instance != NULL)
	{
		delete instance;
	}
}

json FileIO::OpenJSON(std::string filename)
{
	json jsonData;
	std::fstream file(filename);
	if (file.is_open())
	{
		Logging::GetInstance("debugLog.log").LogLine("Opened %s", filename.c_str());
		file >> jsonData;
		file.close();
	}
	else
	{
		Logging::GetInstance("debugLog.log").LogLine("Opened %s", filename.c_str());
		//perror("JSON file does not exist");
		assert("File did not exist");
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

int** FileIO::ReadTiledMap(json jsonData)
{
	if (jsonData["TiledData"].is_object())
	{
		json tiles = jsonData["TiledData"];
		tiles = OpenJSON(tiles["TileMapFile"]);
		//int i = 0, j = 0; //, hatelife = 0; used to debug
		int SizeX = tiles["width"];
		int SizeY = tiles["height"];
		int newindex = 0;

		LevelBuilder::GetInstance()->SetX(SizeX);
		LevelBuilder::GetInstance()->SetY(SizeY);


		if (tiles["layers"].is_array())
		{
			int** TileMap = new int* [SizeX];
			for (int i = 0; i < SizeX; ++i)
			{
				TileMap[i] = new int[SizeY];
			}
			for (auto& layerData : tiles["layers"])
			{
				std::string name = layerData["name"];
				if (name.compare("Walls") != 0)
				{
					if (layerData["data"].is_array())
					{
						json Array = layerData["data"];
						for (int i = 0; i < SizeX; ++i)
						{
							for (int j = 0; j < SizeY; ++j)
							{
								newindex = j * SizeX + i;
								int newVal = Array[newindex];
								TileMap[i][j] = newVal - 1;
								//++hatelife; used to debug json
							}
						}
					}
				}
				if (name.compare("Walls") == 0)
				{
					if (layerData["data"].is_array())
					{
						int* walls = new int[SizeX * SizeY];
						json Array = layerData["data"];
						for (int i = 0; i < SizeX * SizeY; ++i)
						{
							walls[i] = Array[i];
						}
						LevelBuilder::GetInstance()->SetWalls(walls);
					}
				}
			}
			return TileMap;
		}
	}
	return NULL;
}

int** FileIO::ReadTylerTileMap(json jsonData)
{
	json Data = jsonData["TylerTileData"];
	int i = 0, j = 0; //, hatelife = 0; used to debug
	int SizeX = Data["SizeX"];
	int SizeY = Data["SizeY"];

	LevelBuilder::GetInstance()->SetX(SizeX);
	LevelBuilder::GetInstance()->SetY(SizeY);

	int** TileMap = new int* [SizeX];
	for (int i = 0; i < SizeX; ++i)
	{
		TileMap[i] = new int[SizeY];
	}
	if (Data["TileMap"].is_array())
	{
		json Array = Data["TileMap"];
		for (i = 0; i < SizeX; ++i)
		{
			for (j = 0; j < SizeY; ++j)
			{
				if (Array[i][j].is_number())
				{
					TileMap[i][j] = Array[i][j];
					//++hatelife; used to debug json
				}
			}
		}
	}
	return TileMap;
}



FileIO* FileIO::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new FileIO();
	}
	return instance;
}


void FileIO::ReadTileSet(std::string filename, Renderer* pixel, int tileSetType)
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
			if (tileSetType == 0)
			{
				pixel->AddTileToTileset(temp);
			}
			else if(tileSetType == 1)
			{
				pixel->AddNormalToNormalTileset(temp);
			}
			else if (tileSetType == 2)
			{
				pixel->AddShadowCasterToShadowCasterTileset(temp);
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