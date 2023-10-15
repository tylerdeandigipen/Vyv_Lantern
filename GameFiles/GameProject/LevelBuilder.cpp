#include "LevelBuilder.h"
#include "EntityFactory.h"

LevelBuilder* LevelBuilder::instance = new LevelBuilder();

LevelBuilder::LevelBuilder()
{
    entity_container = new EntityContainer();
}

LevelBuilder::~LevelBuilder()
{
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
