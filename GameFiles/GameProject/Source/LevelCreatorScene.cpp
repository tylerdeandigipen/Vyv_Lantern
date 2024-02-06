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

LevelCreatorScene::LevelCreatorScene() : Scene("LevelCreatortest"), tempEntities()
{

}

LevelCreatorScene::~LevelCreatorScene()
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

	LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/LevelCreatorScene.json");


	// init function maps to add files at end
	AddFunc.emplace("Circle", &LevelCreatorScene::AddCircleEntity);
	//AddFunc.emplace("Circle", &AddCircleEntity);

	//AddFunc.emplace("Door", 
	//AddFunc.emplace("Mirror", 

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

void LevelCreatorScene::ExportScene(std::string name)
{
	Renderer* pixel = Renderer::GetInstance();
	int rows = pixel->tileMapSize.x;
	int columns = pixel->tileMapSize.y;

	std::vector<int> tilemap;
	std::vector<int> walls;
	bool found = false;
	for (int i = 0; i < columns; ++i)
	{
		for (int j = 0; j < rows; ++j)
		{
			tilemap.push_back(pixel->tileMap[j][i] + 1);
			for (int k = 0; k < NUM_NON_WALKABLE_TILES; ++k)
			{
				if (pixel->tileMap[j][i] == pixel->nonWalkableTiles[k])
				{
					found = true;
					walls.push_back(pixel->tileMap[j][i]);
				}
			}
			if (found == false)
			{
				walls.push_back(0);
			}
			else
				found = false;
		}
	}

	json tilemapArray;
	json tilemapData;
	json name1;
	json name2;
	json tilemapCol; //collideables

	tilemapArray["width"] = rows;
	tilemapArray["height"] = columns;

	tilemapData["width"] = rows;
	tilemapData["height"] = columns;

	tilemapCol["width"] = rows;
	tilemapCol["height"] = columns;

	tilemapData["name"] = "TileMap";
	tilemapCol["name"] = "Walls";

	tilemapData["data"] = tilemap;
	tilemapCol["data"] = walls;

	tilemapArray["layers"][0] = tilemapData;
	tilemapArray["layers"][1] = tilemapCol;

	// save tilemap to a file
	std::ofstream tileFile("./Data/Scenes/" + name + "MAP" + ".json");
	tileFile << std::setw(2) << tilemapArray << std::endl;

	json readable;
	json data;
	data["TileMapFile"] = "./Data/Scenes/" + name + "MAP" + ".json";
	readable["TiledData"] = data;

	// this will be the object list part
	listToExport = "./Data/Scenes/" + name + "OBJECTS" + ".json";

	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* current = reinterpret_cast<LevelCreatorScene*>(pare);

	for (auto it : tempEntities)
	{
		if (it)
		{
			current->AddFunc[it->addKey](it);
		}
	}
	json ObjectFile;
	ObjectFile["FilePath"] = current->listToExport;
	ObjectFile["Objects"] = gameObjects;

	std::ofstream objectList(listToExport);
	objectList << std::setw(2) << ObjectFile << std::endl;

	readable["GameObjectList"] = { {"GameObjectFile", listToExport } };

	//the filemap to read for the scene 
	std::ofstream actualfile("./Data/Scenes/" + name + ".json");
	actualfile << std::setw(2) << readable << std::endl;
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

	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
	{
		inputHandler->InputKeyClear();
		return;
	}

	bool isAnyEntityPicked = false;

	for (auto& entity : tempEntities)
	{
		if (entity && g_Manager.IsEntityPicked(entity->key))
		{
			isAnyEntityPicked = true;
			break;
		}
	}

	if (!isAnyEntityPicked)
	{
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

	if (!tempEntities.empty())
	{
		for (auto it : tempEntities)
		{
			if (it)
			{
				it->FreeComponents();
				delete it;
				it = NULL;
			}
		}
		tempEntities.clear();
	}
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

	if (showCreatorToolsWindow)
	{
		ImGuiWindow();
	}

	ImGui::Render();
#endif
}

#pragma region Tile Name Converter
auto TileNumToString(unsigned int tileNum) -> std::string
{
	static const std::map<unsigned int, std::string> tileNames =
	{
		{ 0, "Empty" },
		{ 1, "Black" }
	};

	if (tileNum > 1 || tileNum > g_Manager.pRenderer->GetTileCount())
		return "Unknown";

	return tileNames.at(tileNum);
}
#pragma endregion

void LevelCreatorScene::ImGuiWindow()
{
	if (showCreatorToolsWindow)
	{
		ImGui::Begin("Design Tools");
		ImGui::BeginMainMenuBar();

		if (ImGui::MenuItem("Reset Scene", NULL, false, true))
			SceneSystem::GetInstance()->RestartScene();
		if (ImGui::MenuItem("Test Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
		if (ImGui::MenuItem("TbdTest Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(TbdTestSceneGetInstance());

		static bool showTextEditor = false;
		if (ImGui::MenuItem("Text Editor", NULL, false, true))
		{
			if (showTextEditor == true)
				showTextEditor = false;
			else
				showTextEditor = true;
		}
		if (showTextEditor)
			g_Manager.EditText();

		ImGui::EndMainMenuBar();

		ImGui::Text("have fun designers,");
		ImGui::Text("welcome to the creator window!");
		ImGui::Separator();

		ImGui::Text("Scene Settings");
		ImGui::Text("./Data/Scenes/''LevelNameHere''");
		if (ImGui::TreeNode("Export:"))
		{
			ImGui::Text("TileMapOnly");
			ImGui::InputText(".json", myTextBuffer, sizeof(myTextBuffer));
			if (ImGui::Button("ExportTileMap"))
			{
				std::string exportFileName = std::string(myTextBuffer);
				if (exportFileName.empty())
				{
					ImGui::OpenPopup("EmptyExportFileNamePopup");
				}
				else if (!IsFileNameValid(exportFileName))
				{
					ImGui::OpenPopup("InvalidExportFileNamePopup");
				}
				else
				{
					FileIO::GetInstance()->ExportTileMap(myTextBuffer);
				}
			}

			if (ImGui::Button("ExportFullScene"))
			{
				std::string exportFileName = std::string(myTextBuffer);
				if (exportFileName.empty())
				{
					ImGui::OpenPopup("EmptyExportFileNamePopup");
				}
				else if (!IsFileNameValid(exportFileName))
				{
					ImGui::OpenPopup("InvalidExportFileNamePopup");
				}
				else
				{
					ExportScene(myTextBuffer);
				}
			}

			if (ImGui::BeginPopupModal("EmptyExportFileNamePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Please provide a file name for export!");
				if (ImGui::Button("OK"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
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
					if (std::string(filenameBuffer).empty())
					{
						ImGui::OpenPopup("EmptyImportFileNamePopup");
					}
					else if (!IsFileNameValid(filenameBuffer))
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

				if (ImGui::BeginPopupModal("EmptyImportFileNamePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please provide a file name for import!");
					if (ImGui::Button("OK"))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
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
			static bool tool_selected[3] = { false, false, false };

			ImGui::Selectable("Erase 'E' ", &tool_selected[0], ImGuiSelectableFlags_None, ImVec2(100, 25));
			ImGui::Selectable("SquareFill 'shift' ", &tool_selected[1], ImGuiSelectableFlags_None, ImVec2(100, 25));
			ImGui::Selectable("EyeDropper 'alt'", &tool_selected[2], ImGuiSelectableFlags_None, ImVec2(100, 25));

			if (tool_selected[0] == true)
			{
				tool_selected[1] = false;
				tool_selected[2] = false;
				currentTool = 0;
				currentTile = 0;
			}

			if (tool_selected[1] == true)
			{
				tool_selected[0] = false;
				tool_selected[2] = false;
				currentTool = 1;
			}

			if (tool_selected[2] == true)
			{
				tool_selected[0] = false;
				tool_selected[1] = false;
				currentTool = 2;
			}

			if (ImGui::Button("Center 'C' "))
				currentTool = 3;

			switch (currentTool)
			{
			default:
				ImGui::Text("Current Tool: Brush");
				break;
			case 1:
				ImGui::Text("Current Tool: Draw Square");
				break;
			case 2:
				ImGui::Text("Current Tool: Eyedropper");
				break;
			case 3:
				ImGui::Text("Current Tool: Center");
				break;
			}

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Tile Selector:"))
		{
			static int selected = -1;

			for (auto n = 0; n < g_Manager.pRenderer->GetTileCount(); n++)
			{
				if (ImGui::Selectable(("Tile " + TileNumToString(n)).c_str(), selected == n))
					currentTile = n;
			}

			ImGui::Text("Current Tile: %d", currentTile);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	// removing the treenode messed with the spacing, so im adjusting the 
	// spacing so the stuff isnt cuttoff
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
	ImGui::Text("Object Selector:");

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20); // Adjust the value as needed
	if (ImGui::Button("Create Switch"))
	{
		CreateCircleEntity();
	}
	g_Manager.ShowEntityInfo();

	ImGui::Separator();
	g_Manager.EditEntity(Vector2{ static_cast<float>(Inputs::GetInstance()->getMouseX()), static_cast<float>(Inputs::GetInstance()->getMouseY()) });

	ImGui::End();
}

///////////////////////////////////////////////////////////////////////////////////
/* testing stuff ! */

static int circleCount = 0;
int LevelCreatorScene::CreateCircleEntity()
{
	int circles_existing = 0;
	std::string number = "./Data/GameObjects/Circle";
	std::string filename = "./Data/GameObjects/Circle.json";

	Entity* temp = FileIO::GetInstance()->ReadEntity(filename);

	temp->addKey = "Circle"; // this is for the map holding functions and gives access to function for circle

	temp->key = "Circle" + std::to_string(circleCount);
	temp->SetFilePath("./Data/GameObjects/Circle" + std::to_string(circleCount) + ".json");
	tempEntities.push_back(temp);
	++circleCount;
	return 0;
}

void LevelCreatorScene::AddCircleEntity(Entity* entity)
{
	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* current = reinterpret_cast<LevelCreatorScene*>(pare);


	Logging::GetInstance("LevelCreator.log").LogToAll("Creating->", entity->key.c_str());
	json circleData; // the main thing for all pieces to be put inside
	json components; // the components array
	json collider = { {"Type", "ColliderAABB"} };
	json transform = { {"Type", "Transform"}, {"translation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };
	json physics = { {"Type", "Physics"}, {"OldTranslation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };

	components.push_back(collider);
	components.push_back(transform);
	components.push_back(physics);

	circleData["Components"] = components;
	circleData["FilePath"] = entity->GetFilePath();
	circleData["Name"] = "Switch";
	circleData["Type"] = "Object";
	circleData["file"] = "./Assets/PPM/Circle_2x2.ppm";
	circleData["frameSize"] = { 8,8 };
	circleData["isAnimated"] = false;

	std::ofstream circleCreated(entity->GetFilePath());
	circleCreated << std::setw(2) << circleData << std::endl;

	json newobject = { {"FilePath", entity->GetFilePath()} };
	current->gameObjects.push_back(newobject);

}

void LevelCreatorScene::AddToFile(std::string nametoadd, Entity* entity)
{
	if (AddFunc.count(nametoadd))
	{
		AddFunc[nametoadd](entity);
	}
}

// testing something!

void EntityManager::EditEntity(Vector2 mousePos)
{
	if (!pRenderer || !pInput)
		return;

	SetMousePos(mousePos);
	EntityPicker();
}

void EntityManager::ShowEntityInfo()
{
	if (!pRenderer || !pInput)
		return;

	LevelCreatorScene* creator = reinterpret_cast<LevelCreatorScene*>(LevelCreatorSceneGetInstance());
	if (!creator)
		return;

	if (!creator->tempEntities.empty())
	{
		if (ImGui::TreeNode("*Entities:"))
		{
			for (int i = 0; i < creator->tempEntities.size(); i++)
			{
				if (!creator->tempEntities[i])
					continue;

				if (ImGui::TreeNode(("Entity %s", creator->tempEntities[i]->key.c_str())))
				{
					//ImGui::Text("Name: %s", creator->tempEntities[i]->GetName());
					ImGui::Text("Entity Number: %d", i);

					ImGui::Text("Transform: (%f, %f)", properties[creator->tempEntities[i]->key].translation[0], properties[creator->tempEntities[i]->key].translation[1]);
					ImGui::Text("Rotation: (%f, %f)", properties[creator->tempEntities[i]->key].rotation);
					ImGui::SliderInt2("Test Transform", properties[creator->tempEntities[i]->key].translation, -10.f, 100.f);

					ImGui::Checkbox("isEditable", &properties[creator->tempEntities[i]->key].isEditable);
					ImGui::Text((properties[creator->tempEntities[i]->key].isPicked ? "Entity is picked" : "Entity is not picked"));

					if (ImGui::Button("Delete"))
					{
						if (!creator->tempEntities.empty())
						{
							for (auto it = creator->tempEntities.begin(); it != creator->tempEntities.end(); ++it)
							{
								if (*it == creator->tempEntities[i])
								{
									if ((*it)->key.compare(creator->tempEntities[i]->key) == 0)
									{
										(*it)->FreeComponents();
										delete* it;
										*it = nullptr;
										break;
									}
								}
							}
						}
					}

					if (!creator->tempEntities[i])
					{
						continue;
					}
					pRenderer->objects[i]->position.x = properties[creator->tempEntities[i]->key].translation[0];
					pRenderer->objects[i]->position.y = properties[creator->tempEntities[i]->key].translation[1];

					ImGui::TreePop();
				}
			}
		}
	}
}

int EntityManager::ApplyProperties()
{
	if (!pRenderer)
		return EXIT_FAILURE;

	LevelCreatorScene* creator = reinterpret_cast<LevelCreatorScene*>(LevelCreatorSceneGetInstance());
	for (int i = 0; i < creator->tempEntities.size(); i++)
	{
		std::string filename = creator->tempEntities[i]->GetFilePath(); // Replace with your actual JSON file path

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
				component["translation"]["x"] = properties[creator->tempEntities[i]->key].translation[0]; // Replace with the new X value
				component["translation"]["y"] = properties[creator->tempEntities[i]->key].translation[1]; // Replace with the new Y value
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
	}
	return EXIT_SUCCESS;
}

bool EntityManager::InitializeProperties(std::string file_path)
{
	if (!pRenderer)
		return false;

	//initialize level data
	LevelCreatorScene* creator = reinterpret_cast<LevelCreatorScene*>(LevelCreatorSceneGetInstance());
	EntityContainer::GetInstance()->ReadEntities(file_path);

	for (int i = 0; i < creator->tempEntities.size(); i++)
	{
		if (creator->tempEntities[i])
		{
			Transform* t = creator->tempEntities[i]->Has(Transform);
			properties[creator->tempEntities[i]->key] = EntityProperties{ {static_cast<int>(std::floorf(t->GetTranslation()->x)), static_cast<int>(std::floorf(t->GetTranslation()->y))}, {0.f}, false, false };
		}
	}

	file = std::ifstream(fileToEdit);

	if (file.good())
	{
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}
	else
	{
		std::cerr << "Error opening file" << std::endl;
		return false;
	}

	return true; //Initialization success!
}

void EntityManager::EditText()
{
	if (!pRenderer || !pInput)
		return;

	auto cpos = editor.GetCursorPosition();

	editor.SetLanguageDefinition(lang);

	ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(600, 800), ImGuiCond_FirstUseEver);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				auto textToSave = editor.GetText();
				/// save text....
			}
			if (ImGui::MenuItem("Load"))
			{
				ImGui::Begin("Select File", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				ImGui::End();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				editor.SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				editor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor.SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				editor.SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.IsOverwrite() ? "Ovr" : "Ins",
		editor.CanUndo() ? "*" : " ",
		editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

	editor.Render("TextEditor");

	ImGui::End();
}

void EntityManager::EntityPicker()
{
	LevelCreatorScene* creator = reinterpret_cast<LevelCreatorScene*>(LevelCreatorSceneGetInstance());
	for (int i = 0; i < creator->tempEntities.size(); i++)
	{
		if (properties[creator->tempEntities[i]->key].isEditable == true)
		{
			if ((properties[creator->tempEntities[i]->key].translation[0] + 15) >= mousePos_.x && (properties[creator->tempEntities[i]->key].translation[0]) <= mousePos_.x &&
				(properties[creator->tempEntities[i]->key].translation[1] + 15) >= mousePos_.y && (properties[creator->tempEntities[i]->key].translation[1]) <= mousePos_.y)
			{
				if (pInput->mouseButtonDown(SDL_BUTTON_LEFT))
				{
					properties[creator->tempEntities[i]->key].isPicked = true;
				}
				else
				{
					properties[creator->tempEntities[i]->key].isPicked = false;
				}
			}

			if (properties[creator->tempEntities[i]->key].isPicked == true)
			{
				properties[creator->tempEntities[i]->key].translation[0] = mousePos_.x - 8;
				properties[creator->tempEntities[i]->key].translation[1] = mousePos_.y - 8;
			}
		}
	}
}

void EntityManager::SetMousePos(Vector2 mousePos)
{
	mousePos_.x = mousePos.x / pRenderer->screenScale;
	mousePos_.y = mousePos.y / pRenderer->screenScale;
}

bool EntityManager::IsEntityPicked(const std::string& key) const
{
	auto it = properties.find(key);
	return it != properties.end() ? it->second.isPicked : false;
}
