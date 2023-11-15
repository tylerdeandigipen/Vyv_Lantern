#include "LevelBuilder.h"
#include "EntityFactory.h"
#include "FileIO.h"
#include "Transform.h"

LevelBuilder* LevelBuilder::instance = new LevelBuilder();

LevelBuilder::LevelBuilder() : entity_container(new EntityContainer()), jsonData(), TileMap(NULL), Walls(NULL), SizeX(0), SizeY(0)
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
    if (Walls)
    {
        delete[] Walls;
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
            Entity* newEnt = factory->CreateEntity(entityData["Type"], entityData["file"], entityData);
            newEnt->Read(entityData);
            entity_container->AddEntity(newEnt);
            if (newEnt->IsObject())
            {
                if (newEnt->IsAnimated())
                {
                    newEnt->AddToRenderer(pixel, entityData["file"]);
                    //newEnt->Has(Transform)->SetTranslation(&pixel->animatedObjects[0][0]->position);
                    //newEnt->SetImage(pixel->animatedObjects[0][0]);
                }
                else
                {
                    newEnt->AddToRenderer(pixel);
                }
            }
        }
    //}
}

void LevelBuilder::LoadLevel(Renderer* pixel, std::string filename)
{
    std::fstream file(filename);
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
            if (levelData["TylerTileData"].is_object())
            {
                TileMap = FileIO::GetInstance()->ReadTylerTileMap(levelData);
                pixel->tileMapSize.x = static_cast<float>(SizeX);
                pixel->tileMapSize.y = static_cast<float>(SizeY);
                pixel->MakeTileMap(TileMap);

            }
            else
            {
                TileMap = FileIO::GetInstance()->ReadTiledMap(levelData);
                pixel->tileMapSize.x = static_cast<float>(SizeX);
                pixel->tileMapSize.y = static_cast<float>(SizeY);
                pixel->MakeTileMap(TileMap);
            }

            Read(levelData, pixel);
        }
    }
    //pixel->ResizeBuffers();
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
    if (instance)
        delete instance;
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

int** LevelBuilder::GetTileMap()
{
    return TileMap;
}

void LevelBuilder::SetWalls(int* walls)
{
    Walls = walls;
}

int* LevelBuilder::GetWalls()
{
    if (Walls)
    {
        return Walls;
    }
    else
    {
        return NULL;
    }
}

int LevelBuilder::GetX()
{
    return SizeX;
}

int LevelBuilder::GetY()
{
    return SizeY;
}

void LevelBuilder::SetX(int size)
{
    SizeX = size;
}

void LevelBuilder::SetY(int size)
{
    SizeY = size;
}

int LevelBuilder::CountEntities()
{
    return entity_container->CountEntities();
}