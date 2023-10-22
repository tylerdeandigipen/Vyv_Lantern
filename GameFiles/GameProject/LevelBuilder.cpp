#include "LevelBuilder.h"
#include "EntityFactory.h"

LevelBuilder* LevelBuilder::instance = new LevelBuilder();

LevelBuilder::LevelBuilder()
{
    entity_container = new EntityContainer();
}

LevelBuilder::~LevelBuilder()
{
    if (TileMap)
    {
        for (int i = 0; i < SizeX; ++i)
        {
            delete[] TileMap[i];
        }
        delete[] TileMap;
    }
    delete entity_container;
}

Engine::EngineCode LevelBuilder::Init()
{
	return Engine::NothingBad;
}

void LevelBuilder::Read(json const& _jsonData, Renderer* pixel)
{
    EntityFactory* factory = EntityFactory::GetInstance();
    //if (jsonData["Entities"].is_array())
    //{
        for (auto& entityData : _jsonData["Entities"])
        {
            Entity* newEnt = factory->CreateEntity(entityData["Type"], entityData["file"]);
            newEnt->Read(entityData);
            entity_container->AddEntity(newEnt);
            if (newEnt->IsObject())
            {
                newEnt->AddToRenderer(pixel);
            }
        }
    //}
}

void LevelBuilder::LoadLevel(Renderer* pixel)
{
    std::fstream file("./Data/FirstLevel.json");
    if (file.is_open())
    {
        file >> jsonData;
        file.close();
    }
    else
    {
        perror("JSON file does not exist");
    }
    if (jsonData["Levels"].is_array())
    {
        for (auto& levelData : jsonData["Levels"])
        {
            if (levelData["TileData"].is_object())
            {
                json Data = levelData["TileData"];
                int i = 0, j = 0, hatelife = 0;
                SizeX = Data["SizeX"];
                SizeY = Data["SizeY"];

                TileMap = new int *[SizeX];
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
                                ++hatelife;
                            }
                        }
                    }
                }
                pixel->MakeTileMap(TileMap);
            }

            Read(levelData, pixel);
        }
    }
}

void LevelBuilder::LevelUpdate(float dt)
{
    entity_container->UpdateAll(dt);
}

void LevelBuilder::Update(float dt)
{
    dt = dt;
}

void LevelBuilder::Render()
{
}

Engine::EngineCode LevelBuilder::Close()
{
	return Engine::NothingBad;
}

LevelBuilder* LevelBuilder::GetInstance()
{
    return instance;
}


void LevelBuilder::ReLoadLevel()
{
}

void LevelBuilder::FreeLevel()
{
    entity_container->FreeAll();
}
