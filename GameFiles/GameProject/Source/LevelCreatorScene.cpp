//------------------------------------------------------------------------------
//
// File Name:	LevelCreatorScene.cpp
// Author(s):	Tyler Dean, Michael Howard, TayLee Young, 
//              Thomas Stephenson, Louis Wang
// Purpose:     Main scene for the game.
//
// Copyright  ?2023 DigiPen (USA) Corporation.
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
#include "BehaviorPlayer.h"

#include "FileIO.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "Inputs.h"
#include "Logging.h"
#include "Time.h"
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

EntityManager g_Manager;

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

Engine::EngineCode LevelCreatorScene::Init()
{
	LevelCreatorPixelRenderer = Renderer::GetInstance();
	Inputs::GetInstance()->SetWindow(LevelCreatorWindow);

	LevelCreatorWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	LevelCreatorPixelRenderer->window = LevelCreatorWindow;

	if (g_Manager.InitializeProperties("./Data/GameObjects/ObjectListLevelBuilder.json"))
		std::cout << "Property load success!\n";

	LevelBuilder::GetInstance()->LoadTileMap("./Data/TiledMichaelSceneTest.json");

	g_Manager.pRenderer = LevelCreatorPixelRenderer;

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
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
	{
		return Vector2{ 0, 0 };
	}

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
	g_Manager.pInput = inputHandler;

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

bool IsFileNameValid(const std::string& fileName)
{
	static const std::string illegalChars = "#%&{}\\<>*?$!'\":@+`|= ";
	for (char c : fileName)
	{
		if (illegalChars.find(c) != std::string::npos)
		{
			return false;
		}
	}
	return true;
}

void LevelCreatorScene::ImGuiInterg()
{
#ifdef _DEBUG
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	g_Manager.EditText();

	if (showCreatorToolsWindow)
	{
		ImGuiWindow();
	}

	ImGui::Render();
#endif
}

void LevelCreatorScene::ImGuiWindow()
{
	if (showCreatorToolsWindow)
	{
		ImGui::Begin("Design Tools");
		ImGui::Text("have fun designers,");
		ImGui::Text("welcome to the creator window!");

		ImGui::Separator();
		ImGui::Text("Scene Settings");


		ImGui::Text("./Data/Scenes/''LevelNameHere''");

		ImGui::Separator();

		if (ImGui::TreeNode("Export:"))
		{
			ImGui::InputText(".json", myTextBuffer, sizeof(myTextBuffer));
			if (ImGui::Button("Export"))
			{
				std::string exportFileName = std::string(myTextBuffer);
				if (!IsFileNameValid(exportFileName))
				{
					ImGui::OpenPopup("InvalidExportFileNamePopup");
				}
				else
				{
					FileIO::GetInstance()->ExportTileMap(myTextBuffer);
				}
			}

			if (ImGui::BeginPopupModal("InvalidExportFileNamePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Illegal characters in the tilemap name GOOBER!");
				if (ImGui::Button("OK"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}


			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Import:"))
		{
			static char filenameBuffer[256];
			ImGui::InputText(".json", filenameBuffer, sizeof(filenameBuffer));

			ImGui::Text("NOTE: this will reset the current level.");
			{
				if (ImGui::Button("Submit"))
				{
					std::string filename = "./Data/Scenes/" + std::string(filenameBuffer) + ".json";
					if (!IsFileNameValid(filenameBuffer))
					{
						ImGui::OpenPopup("InvalidImportFileNamePopup");
					}
					else
					{
						std::ifstream file(filename);
						if (file.is_open())
						{
							file.close();
							LevelBuilder::GetInstance()->LoadTileMap(filename);
						}
						else
						{
							ImGui::OpenPopup("FileNotFoundPopup");
						}
					}
				}

				if (ImGui::BeginPopupModal("InvalidImportFileNamePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please provide a valid file name without illegal characters for import!");
					if (ImGui::Button("OK"))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("FileNotFoundPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("File not found!");
					if (ImGui::Button("OK"))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			ImGui::TreePop();
		}

		ImGui::Separator();

		if (ImGui::TreeNode("Tools:"))
		{
			if (ImGui::Button("Center 'C' "))
			{
				currentTool = 3;
			}

			if (ImGui::Button("EyeDropper 'alt' "))
			{
				currentTool = 2;
			}

			if (ImGui::Button("Erase 'E' "))
			{
				currentTool = 0;
				currentTile = 0;
			}

			if (ImGui::Button("SquareFill 'shift' "))
			{
				currentTool = 1;
			}

			ImGui::Separator();

			if (ImGui::TreeNode("Entities"))
			{
				auto i = 0;

				if (ImGui::Button("Create Switch"))
				{
					CreateCircleEntity();
				}

				/*if (ImGui::Button("Create Door"))
				{
					CreateDoorEntity();
				}

				if (ImGui::Button("Create Mirror"))
				{
					CreateMirrorEntity();
				} */

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

	if (ImGui::TreeNode("Object Selector:"))
	{
		g_Manager.ShowEntityInfo();

		if (ImGui::Button("Apply Properties"))
		{
			//ApplyProperties(g.);
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

	g_Manager.EditEntity(Vector2{ static_cast<float>(Inputs::GetInstance()->getMouseX()), static_cast<float>(Inputs::GetInstance()->getMouseY()) });

	ImGui::End();


}

///////////////////////////////////////////////////////////////////////////////////
/* testing stuff ! */

int LevelCreatorScene::CreateCircleEntity()
{
	int circles_existing = 0;
	std::string number = "./Data/GameObjects/Circle";
	std::string filename = "./Data/GameObjects/Circle.json";
	std::string line;

	std::ifstream tocopy{ filename };
	if (tocopy.is_open())
	{
		for (int i = 0; i < EntityContainer::GetInstance()->CountEntities(); ++i)
		{
			std::string name = (*EntityContainer::GetInstance())[i]->GetFilePath();
			if (name.compare("./Data/GameObjects/Circle.json") >= 0)
			{
				++circles_existing;
			}
		}

		EntityContainer::GetInstance()->AddEntity(FileIO::GetInstance()->ReadEntity(filename));
		Logging::GetInstance("LevelCreator.log").LogToAll("Timestamped message: %s", "Creating");
		++circles_existing;
		number += std::to_string(circles_existing);
		number += ".json";
		std::ofstream file(number);

		json circleOld = FileIO::GetInstance()->OpenJSON(filename);
		file << std::setw(2) << circleOld << std::endl;

		Logging::GetInstance("LevelCreator.log").LogToAll("Timestamped message: %s", "Copy Finished");

		json circle = FileIO::GetInstance()->OpenJSON(number);
		circle["FilePath"] = number;

	}

	json newobject = { {"FilePath", number} };
	json gameobjects = FileIO::GetInstance()->OpenJSON(currentGameObjectsList);
	if (gameobjects["Objects"].is_array())
	{
		gameobjects["Objects"].push_back(newobject);
		//myarray.push_back(newobject);
	}

	std::ofstream file(currentGameObjectsList);
	file << std::setw(2) << gameobjects << std::endl;

	return 0;
}

/*int LevelCreatorScene::CreateDoorEntity()
{
	std::string filename = "./Data/GameObjects/Door.json";
	EntityContainer::GetInstance()->AddEntity(FileIO::GetInstance()->ReadEntity(filename));
	return 0;
}

int LevelCreatorScene::CreateMirrorEntity()
{
	std::string filename = "./Data/GameObjects/Mirror.json";
	EntityContainer::GetInstance()->AddEntity(FileIO::GetInstance()->ReadEntity(filename));
	return 0;
} */