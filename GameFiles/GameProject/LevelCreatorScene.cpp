//------------------------------------------------------------------------------
//
// File Name:	LevelCreatorScene.cpp
// Author(s):	Tyler Dean, Michael Howard, TayLee Young, 
//              Thomas Stephenson, Louis Wang
// Purpose:     Main scene for the game.
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <algorithm>

#include "LevelCreatorScene.h"
#include "Scene.h"
#include "PlatformSystem.h"
#include "Engine.h"
#include "Entity.h"
#include "BehaviorPlayer.h"
#include "EntityContainer.h"
#include "FileIO.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "Inputs.h"
#include "Logging.h"
#include "Time.h"
#include "Transform.h"

#include "ImageBuffer.h"
#include "Light.h"
#include "LevelBuilder.h"
#include "FontSystem.h"

#include "TestScene.h"
#include "TbdTestScene.h"

Logging& LevelCreatorLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* LevelCreatorRenderer;
Renderer* LevelCreatorPixelRenderer;

SDL_Window* LevelCreatorWindow;

SDL_GLContext LevelCreatorGlContext;

Scene* LevelCreatorSceneinstance = NULL;

static bool showCreatorToolsWindow = true;

bool playMode = false;

bool playerSpawned = false;

struct EntityProperties
{
	float Translation[2] = { 0.f };
	Vector2 Rotation;
	bool isCollidable;
};

LevelCreatorScene::LevelCreatorScene() : Scene("LevelCreatortest")
{

}

Engine::EngineCode LevelCreatorScene::Load()
{
	return Engine::NothingBad;
}

int currentTool = 0;
int currentTile;
int currentObject;

Vector2 moveVector;
Vector2 oldMousePos;
Vector2 previousTile;

EntityContainer* entityContainer;
Entity* entity;
EntityProperties properties;

Engine::EngineCode LevelCreatorScene::Init()
{
	LevelCreatorPixelRenderer = Renderer::GetInstance();
	Inputs::GetInstance()->SetWindow(LevelCreatorWindow);

	LevelCreatorWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	LevelCreatorPixelRenderer->window = LevelCreatorWindow;

	//initialize level data
	EntityContainer::GetInstance()->ReadEntities("./Data/GameObjects/ObjectListLevelBuilder.json");
	entityContainer = EntityContainer::GetInstance();
	entity = (*entityContainer)[0];
	Transform* t = entity->Has(Transform);
	properties = EntityProperties{ {t->GetTranslation()->x, t->GetTranslation()->y}, {0.f, 0.f}, false };

    LevelBuilder::GetInstance()->LoadLevel("./Data/LevelCreatorScene.json");
    LevelCreatorPixelRenderer->window = LevelCreatorWindow;
    LevelCreatorPixelRenderer->isFullbright = true;
    playMode = false;
    playerSpawned = false;

	moveVector = { 0,0 };
	oldMousePos = { 0,0 };
	previousTile = { -1000,-1000 };
	currentTile = 1;
	return Engine::NothingBad;
}

Vector2 pos1, pos2;
bool temp;
bool wasDown = false;
bool reloadTileMap = false;
int expansionRange = 1;

void LevelCreatorScene::ToolEyedroper(Inputs* inputHandler, Vector2 CursourP)
{
	if (inputHandler->mouseButtonDown(SDL_BUTTON_LEFT))
	{
		Vector2 tilePos = (CursourP + LevelCreatorPixelRenderer->GetCameraPosition());
		tilePos /= 8;
		if ((int)tilePos.x <= (int)LevelCreatorPixelRenderer->tileMapSize.x && (int)tilePos.x >= 0 && (int)tilePos.y <= (int)LevelCreatorPixelRenderer->tileMapSize.y && (int)tilePos.y >= 0)
		{
			currentTile = LevelCreatorPixelRenderer->tileMap[(int)tilePos.x][(int)tilePos.y];
		}
		else
		{
			currentTile = 0;
		}
	}
}

void LevelCreatorScene::ToolCenter(Inputs* inputHandler)
{
	moveVector = { 0,0 };
	previousTile = { -1000,-1000 };
	LevelCreatorPixelRenderer->SetCameraPosition(moveVector);
}

void LevelCreatorScene::ToolSquareFill(Inputs* inputHandler, Vector2 CursourP)
{
	Vector2 tilePos = (CursourP + LevelCreatorPixelRenderer->GetCameraPosition());
	tilePos /= 8;
	if (inputHandler->mouseButtonDown(SDL_BUTTON_LEFT) && wasDown == false)
	{
		pos1 = tilePos + PlaceTile(tilePos);
		wasDown = true;
	}
	if (inputHandler->mouseButtonUp(SDL_BUTTON_LEFT) && wasDown == true)
	{
		Vector2 displacement = PlaceTile(tilePos);
		pos2 = tilePos + displacement;
		pos1 += displacement;

		if ((int)pos1.x > (int)pos2.x)
		{
			float temp;
			temp = pos1.x;
			pos1.x = pos2.x;
			pos2.x = temp;
		}
		if ((int)pos1.y > (int)pos2.y)
		{
			float temp;
			temp = pos1.y;
			pos1.y = pos2.y;
			pos2.y = temp;
		}

        for (int x = (int)pos1.x; x < (int)pos2.x + 1; x++)
        {
            for (int y = (int)pos1.y; y < (int)pos2.y + 1; y++)
            {
                if (currentTile != 0)
                {
                    LevelCreatorPixelRenderer->TileMapSetTile(Vector2{ (float)x,(float)y }, currentTile);
                }
                else
                {
                    LevelCreatorPixelRenderer->TileMapEraseTile(Vector2{ (float)x,(float)y });
                }
            }
        }


		for (int x = (int)pos1.x; x < (int)pos2.x + 1; x++)
		{
			for (int y = (int)pos1.y; y < (int)pos2.y + 1; y++)
			{
				if (currentTile != 0 && currentTile != 4)
				{
					LevelCreatorPixelRenderer->TileMapSetTile(Vector2{ (float)x,(float)y }, currentTile);
				}
				else
				{
					LevelCreatorPixelRenderer->TileMapEraseTile(Vector2{ (float)x,(float)y });
				}
			}
		}
		LevelCreatorPixelRenderer->MakeTileMap(LevelCreatorPixelRenderer->tileMap);

		wasDown = false;
	}
}

void LevelCreatorScene::ToolPan(Inputs* inputHandler, Vector2 CursourP)
{
	if (inputHandler->mouseButtonPressed(SDL_BUTTON_RIGHT))
	{
		if (temp == false)
		{
			oldMousePos = CursourP;
		}
		LevelCreatorPixelRenderer->SetCameraPosition(moveVector + (oldMousePos - CursourP));
		temp = true;
	}
	else if (temp)
	{
		temp = false;
		moveVector += oldMousePos - CursourP;
	}
}

void LevelCreatorScene::ToolBrush(Inputs* inputHandler, Vector2 CursourP)
{
	if (inputHandler->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		Vector2 tilePos = (CursourP + LevelCreatorPixelRenderer->GetCameraPosition());
		tilePos /= 8;
		PlaceTile(tilePos);
	}
}

Vector2 LevelCreatorScene::PlaceTile(Vector2 tilePos)
{
	Vector2 displacement = { 0,0 };
	if (tilePos.x < LevelCreatorPixelRenderer->tileMapSize.x && tilePos.x >= 0 && tilePos.y < LevelCreatorPixelRenderer->tileMapSize.y && tilePos.y >= 0)
	{
		if (currentTile == 0)
		{
			LevelCreatorPixelRenderer->TileMapEraseTile(tilePos);
			return displacement;
		}
		LevelCreatorPixelRenderer->TileMapSetTile(tilePos, currentTile);
	}
	else
	{
		if (currentTile == 0)
		{
			return displacement;
		}
		while ((tilePos.x < LevelCreatorPixelRenderer->tileMapSize.x && tilePos.x >= 0 && tilePos.y < LevelCreatorPixelRenderer->tileMapSize.y && tilePos.y >= 0) != true)
		{
			if (tilePos.x < 0)
			{
				LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ -1,0 }, expansionRange);
				tilePos.x += expansionRange;
				displacement.x += expansionRange;
				moveVector.x += expansionRange * TILE_SIZE;
			}
			if (tilePos.x > LevelCreatorPixelRenderer->tileMapSize.x)
			{
				LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 1,0 }, expansionRange + (tilePos.x - LevelCreatorPixelRenderer->tileMapSize.x));
			}
			if (tilePos.y < 0)
			{
				LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 0,-1 }, expansionRange);
				tilePos.y += expansionRange;
				displacement.y += expansionRange;
				moveVector.y += expansionRange * TILE_SIZE;
			}
			if (tilePos.y > LevelCreatorPixelRenderer->tileMapSize.y)
			{
				LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 0,1 }, expansionRange + (tilePos.y - LevelCreatorPixelRenderer->tileMapSize.y));
			}
		}
		LevelCreatorPixelRenderer->MakeTileMap(LevelCreatorPixelRenderer->tileMap);
		LevelCreatorPixelRenderer->TileMapSetTile(tilePos, currentTile);
		previousTile = tilePos;
		LevelCreatorPixelRenderer->SetCameraPosition(moveVector);
	}
	return displacement;
}

bool canTogglePlayMode = true;

void LevelCreatorScene::ToolHandler()
{
	Inputs* inputHandler = Inputs::GetInstance();

	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	Vector2 CursourP = { (float)x, (float)y };
	CursourP *= 1.0f / LevelCreatorPixelRenderer->screenScale;

	if (inputHandler->keyDown(SDL_SCANCODE_SPACE) && canTogglePlayMode)
	{
		if (playMode == false)
		{
			if (playerSpawned == false)
			{
				EntityContainer::GetInstance()->ReadEntities("./Data/GameObjects/PlayerSpawn.json");
				playerSpawned = true;
			}
			LevelCreatorPixelRenderer->animatedObjects[0][0]->position = CursourP + LevelCreatorPixelRenderer->GetCameraPosition() - Vector2{ 3,3 };
			int* walls = new int[LevelCreatorPixelRenderer->tileMapSize.x * LevelCreatorPixelRenderer->tileMapSize.y];

			for (int x = 0; x < LevelCreatorPixelRenderer->tileMapSize.x; ++x)
			{
				for (int y = 0; y < LevelCreatorPixelRenderer->tileMapSize.y; ++y)
				{
					for (int i = 0; i < LevelCreatorPixelRenderer->numNonWalkTiles; ++i)
					{
						walls[(int)(y * LevelCreatorPixelRenderer->tileMapSize.x) + x] = 0;

						if (LevelCreatorPixelRenderer->tileMap[x][y] == LevelCreatorPixelRenderer->nonWalkableTiles[i] || LevelCreatorPixelRenderer->tileMap[x][y] == 0)
						{
							walls[(int)(y * LevelCreatorPixelRenderer->tileMapSize.x) + x] = 1;
							break;
						}
					}
				}
			}
			LevelBuilder::GetInstance()->SetX(LevelCreatorPixelRenderer->tileMapSize.x);
			LevelBuilder::GetInstance()->SetY(LevelCreatorPixelRenderer->tileMapSize.y);
			delete[] LevelBuilder::GetInstance()->GetWalls();
			LevelBuilder::GetInstance()->SetWalls(walls);
			playMode = true;
		}
		else
		{
			playMode = false;
		}
		canTogglePlayMode = false;
	}
	if (inputHandler->keyUp(SDL_SCANCODE_SPACE))
	{
		canTogglePlayMode = true;
	}

	if (playMode != false)
	{
		LevelCreatorPixelRenderer->isFullbright = false;
		LevelCreatorPixelRenderer->animatedObjects[0][0]->isCulled = false;

		//remove this when the player behavior is fixed
		LevelCreatorPixelRenderer->lightSource[1].position = LevelCreatorPixelRenderer->animatedObjects[0][0]->position + Vector2{ 3,3 };
		LevelCreatorPixelRenderer->lightSource[0].position = LevelCreatorPixelRenderer->animatedObjects[0][0]->position + Vector2{ 3,3 };
		
		Vector2 LightP = LevelCreatorPixelRenderer->lightSource[0].position;
		Vector2 D = LightP - CursourP - LevelCreatorPixelRenderer->GetCameraPosition();
		float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;
		LevelCreatorPixelRenderer->lightSource[0].angle = Angle;
		return;
	}

	if (playerSpawned == true)
	{
		LevelCreatorPixelRenderer->animatedObjects[0][0]->isCulled = true;
	}
	LevelCreatorPixelRenderer->isFullbright = true;


	if (inputHandler->keyPressed(SDL_SCANCODE_LSHIFT) && inputHandler->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		//Draw Square
		currentTool = 1;
	}
	else if (inputHandler->keyPressed(SDL_SCANCODE_LALT))
	{
		//Eyedropper
		currentTool = 2;
	}
	else if (inputHandler->mouseButtonPressed(SDL_BUTTON_LEFT) || inputHandler->keyPressed(SDL_SCANCODE_B))
	{
		//Brush
		currentTool = 0;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_E))
	{
		currentTool = 0;
		currentTile = 0;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_C))
	{
		currentTool = 3;
	}

	switch (currentTool)
	{
	default:
		ToolBrush(inputHandler, CursourP);
		break;
	case 1:
		ToolSquareFill(inputHandler, CursourP);
		break;
	case 2:
		ToolEyedroper(inputHandler, CursourP);
		break;
	case 3:
		ToolCenter(inputHandler);
		break;
	}

	ToolPan(inputHandler, CursourP);
}

void LevelCreatorScene::Update(float dt)
{
	if (CheckGameScenes() || CheckRestart())
		return;

	Inputs* inputHandler = Inputs::GetInstance();
	inputHandler->handleInput();
	ImGuiInterg();

	ToolHandler();

	LevelCreatorPixelRenderer->Update(dt);
}

void LevelCreatorScene::Render()
{
	return;
}

Engine::EngineCode LevelCreatorScene::Exit()
{
	EntityContainer::GetInstance()->FreeAll();
	Inputs::GetInstance()->InputKeyClear();
	LevelCreatorPixelRenderer->CleanRenderer();
	return Engine::NothingBad;
}

Engine::EngineCode LevelCreatorScene::Unload()
{
	delete LevelCreatorSceneinstance;
	LevelCreatorSceneinstance = nullptr;
	return Engine::NothingBad;
}

Scene* LevelCreatorSceneGetInstance(void)
{
	static Scene* LevelCreatorSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!LevelCreatorSceneinstance)
	{
		LevelCreatorSceneinstance = new LevelCreatorScene();
	}
	return LevelCreatorSceneinstance;
}

/* --------------------------------------------------------------------------------------------------------------------------------- */

void LevelCreatorScene::ImGuiInterg()
{
#ifdef _DEBUG
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (showCreatorToolsWindow)
	{
		ImGuiWindow();
	}

	ImGui::Render();
#endif
}

int ApplyProperties(EntityProperties properties)
{
	std::string filename = "./Data/GameObjects/Circle.json"; // Replace with your actual JSON file path

	// Read the JSON file
	std::ifstream input_file(filename);
	if (!input_file.is_open())
	{
		std::cerr << "Error opening file" << std::endl;
		return 1;
	}

	json j;
	input_file >> j;
	input_file.close();

	// Modify the translation values
	for (auto& component : j["Components"])
	{
		if (component["Type"] == "Transform")
		{
			// Set new values for x and y
			component["translation"]["x"] = properties.Translation[0]; // Replace with the new X value
			component["translation"]["y"] = properties.Translation[1]; // Replace with the new Y value
			break;
		}
	}

	// Write the modified JSON back to the file
	std::ofstream output_file(filename);
	if (!output_file.is_open())
	{
		std::cerr << "Error opening file for writing" << std::endl;
		return 1;
	}

	output_file << j.dump(4); // Writing with an indentation of 4 spaces
	output_file.close();

	return 0;
}

void LevelCreatorScene::ImGuiWindow()
{
	if (showCreatorToolsWindow)
	{

		ImGui::Begin("Design Tools");
		ImGui::Text("have fun designers,");
		ImGui::Text("welcome to the creator window!");

		ImGui::Separator();

		if (ImGui::TreeNode("Export:"))
		{
			ImGui::InputText(".json", myTextBuffer, sizeof(myTextBuffer));
			if (ImGui::Button("Submit"))
			{
				FileIO::GetInstance()->ExportTileMap(myTextBuffer);
			}
		}


		ImGui::Separator();

		if (ImGui::TreeNode("Tools:"))
		{
			if (ImGui::Button("Center"))
			{
				currentTool = 3;
			}

			if (ImGui::Button("EyeDropper"))
			{
				currentTool = 2;
			}

			if (ImGui::Button("Erase"))
			{
				currentTool = 0;
				currentTile = 0;
			}

			if (ImGui::Button("SquareFill"))
			{
				currentTool = 1;
			}

			if (ImGui::TreeNode("Object Selector:"))
			{
				if (ImGui::TreeNode("All Entities"))
				{
					auto i = 0;
					
				
					if (entity)
					{
						if (ImGui::TreeNode(("Entity %s", entity->GetRealName().c_str())))
						{
							ImGui::Text("Name: %s", entity->GetRealName().c_str());
							ImGui::Text("Entity Number: %d", i);

							ImGui::Text("Transform: (%f, %f)", properties.Translation[0], properties.Translation[1]);
							ImGui::Text("Rotation: (%f, %f)", properties.Rotation);
							ImGui::Checkbox("Apply Collision", &properties.isCollidable);
							ImGui::SliderFloat2("Test Transform", properties.Translation, -10.f, 100.f);
							LevelCreatorPixelRenderer->objects[0]->position.x = properties.Translation[0];
							LevelCreatorPixelRenderer->objects[0]->position.y = properties.Translation[1];
							ApplyProperties(properties);
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}


				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Tile Selector:"))
			{
				if (ImGui::Button("Wall Tile"))
				{
					currentTile = 1;
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::Separator();

		if (ImGui::Button("Reset Scene"))
		{
			SceneSystem::GetInstance()->RestartScene();
		}

		if (ImGui::Button("Test Scene"))
		{
			SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
		}

		if (ImGui::Button("TbdTest Scene"))
		{
			SceneSystem::GetInstance()->SetScene(TbdTestSceneGetInstance());
		}

		ImGui::End();
	}
}