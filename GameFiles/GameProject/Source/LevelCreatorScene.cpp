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
#include "ImGuiManager.h"

#include "LevelCreatorScene.h"
#include "Scene.h"
#include "PlatformSystem.h"
#include "Engine.h"

#include "BehaviorSwitch.h"
#include "BehaviorPlayer.h"
#include "BehaviorEmitter.h"
#include "BehaviorMirror.h"
#include "Entity.h"
#include "Collider.h"
#include "Emitter.h"
#include "LineCollider.h"

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
#include <direct.h>

Logging& LevelCreatorLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* LevelCreatorRenderer;
Renderer* LevelCreatorPixelRenderer;

SDL_Window* LevelCreatorWindow;

SDL_GLContext LevelCreatorGlContext;

Scene* LevelCreatorSceneinstance = NULL;

static bool showCreatorToolsWindow = true;

bool playMode = false;

bool playerSpawned = false;

static int circleCount = 0;
static int mirrorCount = 0;
static int doorCount = 0;
static int emitterCount = 0;
static int ReceiverCount = 0;

int currentTool = 0;
int currentTile;
int currentObject;

Vector2 moveVector;
Vector2 oldMousePos;
Vector2 previousTile;

Vector2 pos1, pos2;
bool temp = false;
bool wasDown = false;
bool reloadTileMap = false;
int expansionRange = 1;

LevelCreatorScene::LevelCreatorScene() : Scene("LevelCreatortest"), playerExists(false), tempEntities()
{
	entityManager = std::make_unique<EntityManager>();
}

LevelCreatorScene::~LevelCreatorScene()
{
}

Engine::EngineCode LevelCreatorScene::Load()
{
	if (entityManager->InitializeProperties("./Data/GameObjects/ObjectListLevelBuilder.json"))
		std::cout << "Property load success!\n";

	return Engine::NothingBad;
}

Engine::EngineCode LevelCreatorScene::Init()
{
	LevelCreatorPixelRenderer = Renderer::GetInstance();
	Inputs::GetInstance()->SetWindow(LevelCreatorWindow);

	LevelCreatorWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	LevelCreatorPixelRenderer->window = LevelCreatorWindow;

	LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/LevelCreatorScene.json");

	// this is gonna add any objects that exist in the imported scene and transfer them to
	// the tempEntities vector so they can be modified (this will have to be added in the import section as well)
	if (EntityContainer::GetInstance()->CountEntities() > 0)
	{
		int size = EntityContainer::GetInstance()->CountEntities();

		for (int i = 0; i < size; ++i)
		{
			if ((*EntityContainer::GetInstance())[i])
			{
				tempEntities.push_back((*EntityContainer::GetInstance())[i]);
				if (tempEntities[i]->GetRealName().compare("Player") == 0)
				{
					playerExists = true;
				}
			}
		}
	}

	// not sure how to do this w/ the new generic vers.
	// init function maps to add files at end
	AddFunc.emplace("Circle", &LevelCreatorScene::AddCircleEntity);
	AddFunc.emplace("Door", &LevelCreatorScene::AddDoorEntity);
	AddFunc.emplace("Mirror", &LevelCreatorScene::AddMirrorEntity);
	AddFunc.emplace("Switch", &LevelCreatorScene::AddMirrorEntity);
	AddFunc.emplace("Emitter", &LevelCreatorScene::AddEmitterEntity);
	AddFunc.emplace("Receiver", &LevelCreatorScene::AddReceiverEntity);

	entityManager->pRenderer = LevelCreatorPixelRenderer;

	moveVector = { 0,0 };
	oldMousePos = { 0,0 };
	previousTile = { -1000,-1000 };
	currentTile = 1;
	return Engine::NothingBad;
}

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
	int rows = (int)pixel->tileMapSize.x;
	int columns = (int)pixel->tileMapSize.y;
	std::string folder = "./Data/Scenes/" + name;
	exportFolder = folder;
	exportFolder += "/";
	if (_mkdir(folder.c_str()) == 0)
	{
		Logging::GetInstance("FailedToMakeFolder.log").LogLine("failed to make file");
	}
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
	std::ofstream tileFile(exportFolder + name + "MAP" + ".json");
	tileFile << std::setw(2) << tilemapArray << std::endl;

	json readable;
	json data;
	data["TileMapFile"] = exportFolder + name + "MAP" + ".json";
	readable["TiledData"] = data;

	// this will be the object list part
	listToExport = exportFolder + name + "OBJECTS" + ".json";

	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* current = reinterpret_cast<LevelCreatorScene*>(pare);

	for (auto it : tempEntities)
	{
		if (it && AddFunc.count(it->addKey))
		{
			current->AddFunc[it->addKey](it);
		}
	}
	json ObjectFile;
	ObjectFile["FilePath"] = current->listToExport;
	ObjectFile["Objects"] = gameObjects;

	ObjectFile["Circle"] = { {"count", circleCount + 1} };
	ObjectFile["Mirror"] = { {"count", mirrorCount + 1} };
	ObjectFile["Door"] = { {"count", doorCount + 1} };
	ObjectFile["Emitter"] = { {"count", emitterCount + 1} };
	ObjectFile["Reviever"] = { {"count", ReceiverCount + 1} };

	std::ofstream objectList(listToExport);
	objectList << std::setw(2) << ObjectFile << std::endl;

	readable["GameObjectList"] = { {"GameObjectFile", listToExport } };

	//the filemap to read for the scene
	std::ofstream actualfile(exportFolder + name + ".json");
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
				LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 1,0 }, expansionRange + (int)(tilePos.x - LevelCreatorPixelRenderer->tileMapSize.x));
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
				LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 0,1 }, expansionRange + (int)(tilePos.y - LevelCreatorPixelRenderer->tileMapSize.y));
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
		if (entity && entityManager->IsEntityPicked(entity->key))
		{
			isAnyEntityPicked = true;
			break;
		}
	}

	if (!isAnyEntityPicked)
	{
		entityManager->pInput = inputHandler;

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
				int* walls = new int[(size_t)LevelCreatorPixelRenderer->tileMapSize.x * (size_t)LevelCreatorPixelRenderer->tileMapSize.y];

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
				LevelBuilder::GetInstance()->SetX((int)LevelCreatorPixelRenderer->tileMapSize.x);
				LevelBuilder::GetInstance()->SetY((int)LevelCreatorPixelRenderer->tileMapSize.y);
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

static void CheckCollisions()
{
	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* level = reinterpret_cast<LevelCreatorScene*>(pare);
	unsigned current = 0;
	while (current != level->tempEntities.size())
	{
		if (level->tempEntities[current])
		{
			Collider* collider = level->tempEntities[current]->Has(Collider);
			if (collider)
			{
				for (unsigned i = current + 1; i < level->tempEntities.size(); ++i)
				{
					if (level->tempEntities[i])
					{
						Collider* secCollider = level->tempEntities[i]->Has(Collider);
						if (secCollider)
						{
							collider->Check(secCollider);
						}

						if (level->tempEntities[current]->Has(Emitter) && level->tempEntities[i]->Has(LineCollider))
						{
							Emitter::EmitterCollisionHandler(*level->tempEntities[current], *level->tempEntities[i]);
						}
					}
				}
			}
		}
		++current;
	}
}
void LevelCreatorScene::Update(float dt)
{
	if (CheckGameScenes() || CheckRestart())
		return;

	int i = 0;
	for (auto it : tempEntities)
	{
		if (it)
		{
			(it)->Update(dt);
			if (it->IsDestroyed())
			{
				it->FreeComponents();
				delete it;
				it = NULL;
			}
		}
		++i;
		CheckCollisions();
	}

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

	circleCount = 0;
	mirrorCount = 0;
	doorCount = 0;
	emitterCount = 0;
	ReceiverCount = 0;

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
	circleCount = 0;
	mirrorCount = 0;
	doorCount = 0;
	emitterCount = 0;
	ReceiverCount = 0;
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

void LevelCreatorScene::ImGuiWindow()
{
	if (showCreatorToolsWindow)
	{
		ImGui::Begin("Design Tools", NULL, ImGuiWindowFlags_NoMove);
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
			entityManager->EditText();

		ImGui::EndMainMenuBar();

		ImGui::Text("have fun designers,");
		ImGui::Text("welcome to the creator window!");
		ImGui::Separator();

		/*if (ImGui::Button("File"))
			ImGui::OpenPopup("my_file_popup");

		ImGui::SetNextWindowPos(ImGui::GetCursorPos());
		if (ImGui::BeginPopup("my_file_popup"))
		{
			ImGuiManager::RenderFilePopup();
			ImGui::EndPopup();
		}*/

		if (ImGui::Button("dont touch!"))
			ImGui::OpenPopup("my_file_popup");
		if (ImGui::BeginPopup("my_file_popup", ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					ImGui::MenuItem("(demo menu)", NULL, false, false);
					if (ImGui::MenuItem("New")) {}
					if (ImGui::MenuItem("Open (import)", "Ctrl+O")) {}
					if (ImGui::MenuItem("Save (export)", "Ctrl+C")) {}
					if (ImGui::BeginMenu("Open Recent"))
					{
						ImGui::MenuItem("yo_mama.cpp");
						ImGui::MenuItem("horse_react.h");
						ImGui::MenuItem("b-man.tyler");

						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					ImGui::MenuItem("why are you here?");
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Text("this is a test!");
			ImGui::Button("these all do nothing...");
			ImGui::EndPopup();
		}

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
					ImGui::OpenPopup("SuccessfulExport");
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
					ImGui::OpenPopup("SuccessfulExport");
				}
			}

			ImGuiManager::RenderOKPopup("SuccessfulExport", "Exported!");
			ImGuiManager::RenderOKPopup("EmptyExportFileNamePopup", "Please provide a file name for export!");
			ImGuiManager::RenderOKPopup("InvalidExportFileNamePopup", "Illegal characters in the tilemap name GOOBER!");

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
						sceneName = std::string(filenameBuffer);
						if (file.is_open())
						{
							file.close();
							Renderer::GetInstance()->CleanRenderer();
							LevelBuilder::GetInstance()->LoadTileMap(filename);
							if (EntityContainer::GetInstance()->CountEntities() > 0)
							{
								int size = EntityContainer::GetInstance()->CountEntities();
								for (int i = 0; i < size; ++i)
								{
									if ((*EntityContainer::GetInstance())[i])
									{
										tempEntities.push_back((*EntityContainer::GetInstance())[i]);
										tempEntities[i]->addKey = tempEntities[i]->GetRealName();
										if (tempEntities[i]->GetRealName().compare("Player") == 0)
										{
											playerExists = true;
										}
									}
								}
							}
							json counts = FileIO::GetInstance()->OpenJSON("./Data/Scenes/" + std::string(filenameBuffer) + "OBJECTS.json");

							if (counts["Circle"].is_object())
							{
								circleCount = counts["Circle"]["count"];
							}
							if (counts["Door"].is_object())
							{
								doorCount = counts["Door"]["count"];
							}
							if (counts["Mirror"].is_object())
							{
								mirrorCount = counts["Mirror"]["count"];
							}if (counts["Emitter"].is_object())
							{
								emitterCount = counts["Emitter"]["count"];
							}if (counts["Receiver"].is_object())
							{
								ReceiverCount = counts["Receiver"]["count"];
							}

							if (!entityManager->InitializeProperties(filename + "OBJECTS.json"))
							{
								ImGui::OpenPopup("NoObjectsInScene");
							}

							ImGui::OpenPopup("SuccessfulImport");
						}
						else
						{
							ImGui::OpenPopup("FileNotFoundPopup");
						}
					}
				}

				ImGuiManager::RenderOKPopup("SuccessfulImport", "Successfully imported the scene!");

				ImGuiManager::RenderOKPopup("NoObjectsInScene", "Objects do not exist in this scene.");

				ImGuiManager::RenderOKPopup("EmptyImportFileNamePopup", "Please provide a file name for import!");

				ImGuiManager::RenderOKPopup("InvalidImportFileNamePopup", "Please provide a valid file name without illegal characters for import!");

				ImGuiManager::RenderOKPopup("InvalidImportFileNamePopup", "Please provide a valid file name without illegal characters for import!");

				ImGuiManager::RenderOKPopup("FileNotFoundPopup", "File not found!");
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

		static int selected = -1;

		if (ImGui::Button("Select Tile"))
			ImGui::OpenPopup("Tile Selector");

		ImGui::SameLine();
		ImGui::Text("Current Tile: %s", TileNumToString(selected).c_str());

		if (ImGui::BeginPopup("Tile Selector"))
		{
			for (unsigned n = 0; n < entityManager->pRenderer->GetTileCount(); n++)
			{
				if (ImGui::Selectable(TileNumToString(n).c_str(), selected == n))
				{
					selected = n;
					currentTile = n; // Update the currentTile when a tile is selected
				}
			}
			ImGui::EndPopup();
		}
	}

	// removing the treenode messed with the spacing, so im adjusting the
	// spacing so the stuff isnt cuttoff
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Object Selector:");
	if (ImGui::Button(" Create Player"))
	{
		if (playerExists == true)
		{
			ImGui::OpenPopup("PlayerAlreadyExists");
		}
		else
		{
			CreatePlayerEntity();
		}
	}

	ImGuiManager::RenderOKPopup("PlayerAlreadyExists", "Player Object Already in Scene!");
	if (ImGui::Button("  Create Switch"))
	{
		CreateCircleEntity();
	}
	if (ImGui::Button("  Create Door"))
	{
		CreateDoorEntity();
	}
	MirrorData scaredConfused = ImGuiManager::RenderMirrorDirPopup("MirrorDirection", "Pick a mirror direction.");
	if (ImGui::Button("  Create Mirror") || (scaredConfused.spriteDirection > 0 && scaredConfused.done))
	{
		if (!scaredConfused.done)
			ImGui::OpenPopup("MirrorDirection");

		//ImGuiManager::MirrorCheckBox(scaredConfused);
		if (scaredConfused.spriteDirection > 0 && scaredConfused.done)
		{
			CreateMirrorEntity(scaredConfused);
			scaredConfused = MirrorData(); // reset values
		}
	}
	EmitterData emitterDirection = ImGuiManager::RenderEmitterDirPopup("EmitterDirection", "Pick a Emitter direction.");
	if (ImGui::Button("  Create Emitter") || (emitterDirection.spriteDirection > 0 && emitterDirection.done))
	{
		if (!emitterDirection.done)
			ImGui::OpenPopup("EmitterDirection");
		if (emitterDirection.spriteDirection > 0 && emitterDirection.done)
		{
			CreateEmitterEntity(emitterDirection); // temporary value untill i fix it
			emitterDirection = EmitterData(); // reset values
		}
	}

	if (ImGui::Button("  Create Receiver"))
	{
		CreateReceiverEntity();
	}

	ImGui::Separator();
	entityManager->EditEntity(Vector2{ static_cast<float>(Inputs::GetInstance()->getMouseX()), static_cast<float>(Inputs::GetInstance()->getMouseY()) });

	entityManager->ShowEntityInfo();

	ImGui::End();
}

///////////////////////////////////////////////////////////////////////////////////
/* testing stuff ! */

// not deleting in case the generic ver. doesnt work -- we have backup!

// also -- thanks won for getting them in, i just made it generic lol

/* this will need to be modified because its just gonna keep overwriting the player object that already exists
   unless the scene does become open world then its gonna be fine */

static int playerCount = 0;
int LevelCreatorScene::CreatePlayerEntity()
{
	int circles_existing = 0;
	playerExists = true;
	std::string number = "./Data/GameObjects/Player";
	std::string filename = "./Data/GameObjects/Player.json";

	Entity* temp = FileIO::GetInstance()->ReadEntity(filename);

	temp->addKey = "Player"; // this is for the map holding functions and gives access to function for circle

	temp->key = "Player" + std::to_string(playerCount);
	temp->SetFilePath("./Data/GameObjects/Player" + std::to_string(playerCount) + ".json");
	tempEntities.push_back(temp);
	++playerCount;

	return 0;
}

//static int circleCount = 0;
int LevelCreatorScene::CreateCircleEntity()
{
	int circles_existing = 0;
	std::string number = "./Data/GameObjects/Circle";
	std::string filename = "./Data/GameObjects/Circle.json";

	Entity* temp = FileIO::GetInstance()->ReadEntity(filename);

	temp->addKey = "Circle"; // this is for the map holding functions and gives access to function for circle

	temp->key = "Circle" + std::to_string(circleCount);
	tempEntities.push_back(temp);
	++circleCount;
	return 0;
}

//static int doorCount = 0;
int LevelCreatorScene::CreateDoorEntity()
{
	int door_existing = 0;
	std::string number = "./Data/GameObjects/Door";
	std::string filename = "./Data/GameObjects/Door.json";

	Entity* temp = FileIO::GetInstance()->ReadEntity(filename);

	temp->addKey = "Door"; // this is for the map holding functions and gives access to function for circle

	temp->key = "Door" + std::to_string(doorCount);

	//temp->SetFilePath("./Data/GameObjects/Door" + sceneName + std::to_string(doorCount) + ".json");
	tempEntities.push_back(temp);
	++doorCount;
	return 0;
}

//static int mirrorCount = 0;
int LevelCreatorScene::CreateMirrorEntity(MirrorData mirror)
{
	int door_existing = 0;

	UNREFERENCED_PARAMETER(door_existing);

	std::string number = "./Data/GameObjects/Mirror";

	std::string filename;
	switch (mirror.spriteDirection) {
	case 1: filename = "./Data/GameObjects/MirrorTopLeft.json"; break;
	case 2: filename = "./Data/GameObjects/MirrorTopRight.json"; break;
	case 3: filename = "./Data/GameObjects/MirrorBottomLeft.json"; break;
	case 4: filename = "./Data/GameObjects/MirrorBottomRight.json"; break;
	default: return 0;
	}

	Entity* temp = FileIO::GetInstance()->ReadEntity(filename);
	temp->GetComponent<BehaviorMirror>()->SetReflection(mirror.direction);

	//temp->GetComponent<BehaviorMirror>()->SetColor(mirror.newcolor);
	temp->addKey = "Mirror"; // this is for the map holding functions and gives access to function for circle

	temp->key = "Mirror" + std::to_string(mirrorCount);

	//temp->SetFilePath("./Data/GameObjects/Mirror" + sceneName + std::to_string(mirrorCount) + ".json");
	tempEntities.push_back(temp);
	++mirrorCount;
	return 0;
}

//static int emitterCount = 0;
int LevelCreatorScene::CreateEmitterEntity(EmitterData emit)
{
	std::string number = "./Data/GameObjects/Emitter";
	std::string filename = "./Data/GameObjects/tempEmitter.json";

	Entity* temp = FileIO::GetInstance()->ReadEntity(filename);

	//std::string filename;
	switch (emit.spriteDirection) {
	case 1:
	{
		BehaviorEmitter* mine = temp->GetComponent<BehaviorEmitter>();
		mine->SetDirection({ 0.0f, -1.0f });
		mine->SetPosition(*temp->GetComponent<Transform>()->GetTranslation());
		break;
	}
	case 2:
	{
		BehaviorEmitter* mine = temp->GetComponent<BehaviorEmitter>();
		mine->SetPosition(*temp->GetComponent<Transform>()->GetTranslation());
		mine->SetDirection({ 0.0f, 1.0f });
		break;
	}
	case 3:
	{
		BehaviorEmitter* mine = temp->GetComponent<BehaviorEmitter>();
		mine->SetDirection({ 1.0f, 0.0f });
		mine->SetPosition(*temp->GetComponent<Transform>()->GetTranslation());
		break;
	}
	case 4:
	{
		BehaviorEmitter* mine = temp->GetComponent<BehaviorEmitter>();
		mine->SetDirection({ -1.0f, 0.0f });
		mine->SetPosition(*temp->GetComponent<Transform>()->GetTranslation());
		break;
	}
	default: return 0;
	}
	temp->addKey = "Emitter"; // this is for the map holding functions and gives access to function for circle

	temp->key = "Emitter" + std::to_string(emitterCount);

	//temp->SetFilePath("./Data/GameObjects/Emitter"+ sceneName + std::to_string(emitterCount) + ".json");
	tempEntities.push_back(temp);
	++emitterCount;
	return 0;
}

//static int ReceiverCount = 0;
int LevelCreatorScene::CreateReceiverEntity()
{
	std::string number = "./Data/GameObjects/Receiver";
	std::string filename = "./Data/GameObjects/tempReceiver.json";

	Entity* temp = FileIO::GetInstance()->ReadEntity(filename);
	temp->addKey = "Receiver"; // this is for the map holding functions and gives access to function for circle
	temp->key = "Receiver" + std::to_string(emitterCount);

	//temp->SetFilePath("./Data/GameObjects/Receiver" + sceneName + std::to_string(emitterCount) + ".json");
	tempEntities.push_back(temp);
	++ReceiverCount;
	return 0;
}

/* these look like a lot of copy and paste code but for now its so that json can be exported easier
   in order to get it generalized we'd have to just check for the right components */
void LevelCreatorScene::AddCircleEntity(Entity* entity)
{
	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* current = reinterpret_cast<LevelCreatorScene*>(pare);
	entity->SetFilePath(current->exportFolder + entity->key + ".json");

	Logging::GetInstance("LevelCreator.log").LogToAll("Creating->", entity->key.c_str());
	json circleData;
	json components;
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
	circleData["file"] = entity->spritePath;
	circleData["frameSize"] = { 8,8 };
	circleData["isAnimated"] = false;

	std::ofstream circleCreated(entity->GetFilePath());
	circleCreated << std::setw(2) << circleData << std::endl;

	json newobject = { {"FilePath", entity->GetFilePath()} };
	current->gameObjects.push_back(newobject);
}

void LevelCreatorScene::AddDoorEntity(Entity* entity)
{
	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* current = reinterpret_cast<LevelCreatorScene*>(pare);
	entity->SetFilePath(current->exportFolder + entity->key + ".json");

	Logging::GetInstance("LevelCreator.log").LogToAll("Creating->", entity->key.c_str());
	json doorData; // the main thing for all pieces to be put inside
	json components; // the components array
	json collider = { {"Type", "ColliderAABB"} };
	json transform = { {"Type", "Transform"}, {"translation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };
	json physics = { {"Type", "Physics"}, {"OldTranslation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };
	json behaivor = { {"Type", "BehaviorDoor"}, {"DoorClosed", true}, {"ClosedSprite", "./Assets/PPM/Door_Closed.ppm"}, {"OpenSprite", "./Assets/PPM/Door_Open.ppm"}, {"PairName", entity->GetComponent<BehaviorDoor>()->GetPairName() } };

	components.push_back(collider);
	components.push_back(transform);
	components.push_back(physics);
	components.push_back(behaivor);

	doorData["Components"] = components;
	doorData["FilePath"] = entity->GetFilePath();
	doorData["Name"] = "Door";
	doorData["Type"] = "Object";
	doorData["file"] = entity->spritePath;
	doorData["frameSize"] = { 8,8 };
	doorData["isAnimated"] = false;
	doorData["KeyObject"] = { {"key", entity->key} };

	std::ofstream doorCreated(entity->GetFilePath());
	doorCreated << std::setw(2) << doorData << std::endl;

	json newobject = { {"FilePath", entity->GetFilePath()} };
	current->gameObjects.push_back(newobject);
}

void LevelCreatorScene::AddMirrorEntity(Entity* entity)
{
	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* current = reinterpret_cast<LevelCreatorScene*>(pare);
	entity->SetFilePath(current->exportFolder + entity->key + ".json");

	Logging::GetInstance("LevelCreator.log").LogToAll("Creating->", entity->key.c_str());
	json mirrorData; // the main thing for all pieces to be put inside
	json components; // the components array
	json collider = { {"Type", "ColliderAABB"} };
	json transform = { {"Type", "Transform"}, {"translation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };
	json physics = { {"Type", "Physics"}, {"OldTranslation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };
	BehaviorSwitch* entSwitch = entity->GetComponent<BehaviorSwitch>();
	json bSwitch;
	if (entSwitch && entSwitch->GetKey() >= 0)
	{
		bSwitch = { {"Type", "BehaviorSwitch"}, { "key", entSwitch->GetKey() },{"NumPositions", entSwitch->GetMaxCount() + 1} };
		for (int i = 0; i < entSwitch->GetMaxCount() + 1; ++i)
		{
			json pos = { {"x", std::to_string((*entSwitch)[i].x)}, {"y", std::to_string((*entSwitch)[i].y)} };
			bSwitch["pos"].push_back(pos);
		}
	}
	BehaviorMirror* entMirr = entity->GetComponent<BehaviorMirror>();
	json bMirror;
	if (entMirr)
	{
		json direction = { {"DirectionX", entity->GetComponent<BehaviorMirror>()->GetReflectDirection().x}, {"DirectionY", entity->GetComponent<BehaviorMirror>()->GetReflectDirection().y} };
		bMirror = { {"Type", "BehaviorMirror"}, {"Direction", direction}, {"key", 0} }; // will add color if it comes up again but will not for now
	}

	components.push_back(collider);
	components.push_back(transform);
	components.push_back(physics);
	if (entSwitch)
		components.push_back(bSwitch);
	if (entMirr)
		components.push_back(bMirror);
	mirrorData["Components"] = components;
	mirrorData["FilePath"] = entity->GetFilePath();
	mirrorData["Name"] = "Switch";
	mirrorData["Type"] = "Object";
	mirrorData["file"] = entity->spritePath;
	mirrorData["frameSize"] = { 8,8 };
	mirrorData["isAnimated"] = false;
	mirrorData["KeyObject"] = { {"key", entity->key} };

	std::ofstream doorCreated(entity->GetFilePath());
	doorCreated << std::setw(2) << mirrorData << std::endl;

	json newobject = { {"FilePath", entity->GetFilePath()} };
	current->gameObjects.push_back(newobject);
}

void LevelCreatorScene::AddEmitterEntity(Entity* entity)
{
	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* current = reinterpret_cast<LevelCreatorScene*>(pare);
	entity->SetFilePath(current->exportFolder + entity->key + ".json");

	Logging::GetInstance("LevelCreator.log").LogToAll("Creating->", entity->key.c_str());
	json mirrorData; // the main thing for all pieces to be put inside
	json components; // the components array
	json collider = { {"Type", "ColliderAABB"} };
	json transform = { {"Type", "Transform"}, {"translation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };
	json physics = { {"Type", "Physics"}, {"OldTranslation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };

	json emitter;
	BehaviorEmitter* entEmit = entity->GetComponent<BehaviorEmitter>();
	if (entEmit)
	{
		json direction = { {"x", entity->GetComponent<BehaviorEmitter>()->GetDirection().x}, {"y", entity->GetComponent<BehaviorEmitter>()->GetDirection().y} };
		emitter = { {"Type", "BehaviorEmitter"}, {"direction", direction} }; // will add color if it comes up again but will not for now
	}

	components.push_back(collider);
	components.push_back(transform);
	components.push_back(physics);
	if (entEmit)
		components.push_back(emitter);
	mirrorData["Components"] = components;
	mirrorData["FilePath"] = entity->GetFilePath();
	mirrorData["Name"] = "Emitter";
	mirrorData["Type"] = "Object";
	mirrorData["file"] = entity->spritePath;
	mirrorData["frameSize"] = { 8,8 };
	mirrorData["isAnimated"] = false;

	mirrorData["KeyObject"] = { {"key", entity->key} };

	std::ofstream doorCreated(entity->GetFilePath());
	doorCreated << std::setw(2) << mirrorData << std::endl;

	json newobject = { {"FilePath", entity->GetFilePath()} };
	current->gameObjects.push_back(newobject);
}

void LevelCreatorScene::AddReceiverEntity(Entity* entity)
{
	Scene* pare = LevelCreatorSceneGetInstance();
	LevelCreatorScene* current = reinterpret_cast<LevelCreatorScene*>(pare);
	entity->SetFilePath(current->exportFolder + entity->key + ".json");

	Logging::GetInstance("LevelCreator.log").LogToAll("Creating->", entity->key.c_str());
	json mirrorData; // the main thing for all pieces to be put inside
	json components; // the components array
	json collider = { {"Type", "ColliderAABB"} };
	json transform = { {"Type", "Transform"}, {"translation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };
	json physics = { {"Type", "Physics"}, {"OldTranslation", { { "x", entity->Has(Transform)->GetTranslation()->x }, {"y", entity->Has(Transform)->GetTranslation()->y} } } };
	json receiver = { {"Type", "BehaviorReceiver"} };

	components.push_back(collider);
	components.push_back(transform);
	components.push_back(physics);
	components.push_back(receiver);

	mirrorData["Components"] = components;
	mirrorData["FilePath"] = entity->GetFilePath();
	mirrorData["Name"] = "Receiver";
	mirrorData["Type"] = "Object";
	mirrorData["file"] = entity->spritePath;
	mirrorData["frameSize"] = { 8,8 };
	mirrorData["isAnimated"] = false;
	mirrorData["KeyObject"] = { {"key", entity->key} };

	std::ofstream doorCreated(entity->GetFilePath());
	doorCreated << std::setw(2) << mirrorData << std::endl;

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