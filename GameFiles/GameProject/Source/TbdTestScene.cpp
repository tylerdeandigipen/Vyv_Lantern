//------------------------------------------------------------------------------
//
// File Name:	TbdTestScene.cpp
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

#include "TbdTestScene.h"
#include "LevelCreatorScene.h"
#include "Scene.h"
#include "PlatformSystem.h"
#include "Engine.h"
#include "Entity.h"
#include "EntityContainer.h"
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
#include "Section1Final.h"

#include "DebugNew.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Logging& TbdLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* TbdRenderer;
Renderer* TbdPixelRenderer = Renderer::GetInstance();

SDL_Window* TbdWindow;

SDL_GLContext TbdGlContext;

Scene* TbdTestSceneinstance = NULL;

laser_emitter* TestLaser;

TbdTestScene::TbdTestScene() : Scene("tbdtest")
{
	entityManagerTBD = std::make_unique<EntityManager>();
}

Engine::EngineCode TbdTestScene::Load()
{
	AudioManager.LoadMusicFromJSON("./Data/music.json");//line is good
	AudioManager.LoadSFXFromJSON("./Data/SFX.json");// line is goodplay

	if (entityManagerTBD->InitializeProperties("./Data/GameObjects/TutorialFinalActual/TutorialFinalActualOBJECTS.json"))

		//if (entityManagerTBD->InitializeProperties("./Data/Scenes/TutorialFinalActual/TutorialFinalActualOBJECTS.json"))
		std::cout << "Property load success!\n";

	return Engine::NothingBad;
}

Engine::EngineCode TbdTestScene::Init()
{
	Inputs::GetInstance()->SetWindow(TbdWindow);

	//exporttests
	//FileIO::GetInstance()->ExportTileMap("export_tests");

	Light tempLight;
	Light tempLight2;

	// Create SDL Window
	TbdWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	TbdPixelRenderer->window = TbdWindow;

	//initialize level data
	//EntityContainer::GetInstance()->ReadEntities();
	LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/TutorialFinalActual/TutorialFinalActual.json");

	//LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/TutorialFinalActual/TutorialFinalActual.json");

	//ControlledEmitter = LaserSystem::GetInstance()->CreateEmitter();
	/*
	laser_emitter* TestLaser1 = NewEmitter();
	TestLaser1->Position = Vector2(200.0f, 50.0f + 12.0f); //add offset to accomodate for the position of the mirror (opengl position mixed with sdl2 position)
	TestLaser1->Direction = Vector2::Normalize(Vector2(-1.0f, 0.0f));
	*/
	Color tempColor{ 141,141,141,255 };
	int numTestDust = 120;
	Vector2 tempRandNum;
	for (int i = 0; i < numTestDust; i++)
	{
		tempRandNum = Vector2{ (float)(rand() % SCREEN_SIZE_X - 10), (float)(rand() % SCREEN_SIZE_Y - 10) };
		tempRandNum += Vector2{ 10,10 };
		Particle* testParticle = new Particle(tempRandNum, Vector2{ -0.8f,-0.25f }, Vector2{ 17.0f, 15.0f }, tempColor, Particle_Dust);
		TbdPixelRenderer->particleManager->AddParticle(testParticle);
	}

	//AudioManager.PlayMusic("drips"); //line is good

	//AudioManager.PlayMusic("forest"); //line is good
	AudioManager.PlayMusic("bgm");
	AudioManager.DecreaseMusicVolume(0.7f);

	AudioManager.PlaySFX("bgAmbience");

	//AudioManager.PlaySFX("laserAmbience"); // replace with things that sound more "electric humming"

	FontSystem fontSystem;

	fontSystem.init("Font/MouldyCheeseRegular-WyMWG.ttf", 10);

	entityManagerTBD->pRenderer = TbdPixelRenderer;

	Engine::GetInstance()->SetPause(false);
	return Engine::NothingBad;
}

#ifndef Render_Toggle_Functions
int TbdCanPlaceLight = 0;
bool TbdCanToggleFullBright = true;
bool TbdCanToggleNormalDisplay = true;
bool TbdCanToggleOnlyLights = true;
bool CanPause = true;
bool canToggleScanLines = true;
bool TbdCanRenderWallHitboxes = true;
bool canRenderRawFog = true;
bool canToggleFog = true;

static bool tabKeyPreviouslyPressed = false;
static bool show_demo_window = false;
static bool show_tool_metrics = false;
static bool show_custom_window = false;
static bool show_metrics_debug_bar = false;

static bool isGravePressedForCheat = false;
static bool isQPressedForCheat = false;
static bool isNPressedForCheat = false;
static bool isCPressedForCheat = false;
static bool isFPressedForCheat = false;
static bool isEPressedForCheat = false;
static bool isHPressedForCheat = false;
static bool isGPressedForCheat = false;

void TbdTestScene::cheatFullbright()
{
	if (TbdPixelRenderer->isFullbright == false)
		TbdPixelRenderer->isFullbright = true;
	else
		TbdPixelRenderer->isFullbright = false;
	TbdCanToggleFullBright = false;
}

void TbdTestScene::cheatBlur()
{
	if (TbdPixelRenderer->doBlur == false)
		TbdPixelRenderer->doBlur = true;
	else
		TbdPixelRenderer->doBlur = false;
	TbdCanPlaceLight = 0;
}

void TbdTestScene::cheatOnlyLights()
{
	if (TbdPixelRenderer->renderOnlyLights == false)
		TbdPixelRenderer->renderOnlyLights = true;
	else
		TbdPixelRenderer->renderOnlyLights = false;
	TbdCanToggleOnlyLights = false;
}

void TbdTestScene::cheatNormalMap()
{
	if (TbdPixelRenderer->renderNormalMap == false)
		TbdPixelRenderer->renderNormalMap = true;
	else
		TbdPixelRenderer->renderNormalMap = false;
	TbdCanToggleNormalDisplay = false;
}

void TbdTestScene::cheatWallHitboxes()
{
	if (TbdPixelRenderer->renderWallHitboxes == false)
		TbdPixelRenderer->renderWallHitboxes = true;
	else
		TbdPixelRenderer->renderWallHitboxes = false;
	TbdCanRenderWallHitboxes = 0;
}

void TbdTestScene::cheatScanlines()
{
	if (TbdPixelRenderer->doScanLines == false)
		TbdPixelRenderer->doScanLines = true;
	else
		TbdPixelRenderer->doScanLines = false;
	canToggleScanLines = 0;
}

void TbdTestScene::cheatOnlyFog()
{
	if (TbdPixelRenderer->drawRawFog == false)
		TbdPixelRenderer->drawRawFog = true;
	else
		TbdPixelRenderer->drawRawFog = false;
	canRenderRawFog = 0;
}

void TbdTestScene::cheatFog()
{
	if (TbdPixelRenderer->doFog == false)
		TbdPixelRenderer->doFog = true;
	else
		TbdPixelRenderer->doFog = false;
	canToggleFog = 0;
}

void TbdTestScene::handleCheatCodes()
{
	Inputs* inputHandler = Inputs::GetInstance();

	if (inputHandler->keyPressed(SDL_SCANCODE_ESCAPE))
	{
		if (CanPause == true)
		{
			if (Engine::GetInstance()->Paused() == false)
			{
				AudioManager.PlaySFX("pauseOpen");
				Engine::GetInstance()->SetPause(true);
			}
			else
			{
				AudioManager.PlaySFX("pauseClose");
				Engine::GetInstance()->SetPause(false);
			}
		}
		CanPause = false;
	}
	else
	{
		CanPause = true;
	}
#ifdef _DEBUG
	if (inputHandler->keyPressed(SDL_SCANCODE_GRAVE) && TbdCanToggleFullBright == true)
	{
		cheatFullbright();

		if (!isGravePressedForCheat)
		{
			isGravePressedForCheat = true;
		}
		else
		{
			isGravePressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_GRAVE))
	{
		TbdCanToggleFullBright = true;
	}
	if (inputHandler->keyPressed(SDL_SCANCODE_7))
	{
		SceneSystem::GetInstance()->SetScene(Section1FinalGetInstance());
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_Q) && TbdCanToggleOnlyLights == true)
	{
		cheatOnlyLights();

		if (!isQPressedForCheat)
		{
			isQPressedForCheat = true;
		}
		else
		{
			isQPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_Q))
	{
		TbdCanToggleOnlyLights = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_N) && TbdCanToggleNormalDisplay == true)
	{
		cheatNormalMap();

		if (!isNPressedForCheat)
		{
			isNPressedForCheat = true;
		}
		else
		{
			isNPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_N))
	{
		TbdCanToggleNormalDisplay = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_C) && TbdCanRenderWallHitboxes == 1)
	{
		cheatWallHitboxes();

		if (!isCPressedForCheat)
		{
			isCPressedForCheat = true;
		}
		else
		{
			isCPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_C))
	{
		TbdCanRenderWallHitboxes = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_F) && canToggleScanLines == 1)
	{
		cheatScanlines();

		if (!isFPressedForCheat)
		{
			isFPressedForCheat = true;
		}
		else
		{
			isFPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_F))
	{
		canToggleScanLines = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_H) && canRenderRawFog == 1)
	{
		cheatOnlyFog();

		if (!isHPressedForCheat)
		{
			isHPressedForCheat = true;
		}
		else
		{
			isHPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_H))
	{
		canRenderRawFog = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_G) && canToggleFog == 1)
	{
		cheatFog();

		if (!isGPressedForCheat)
		{
			isGPressedForCheat = true;
		}
		else
		{
			isGPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_G))
	{
		canToggleFog = 1;
	}
#endif _DEBUG
}

#endif

bool isMuted = false;
bool mKeyDown = false;
void TbdPlayerMovement(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	Inputs* inputHandler = Inputs::GetInstance();

#ifdef _DEBUG
	if (inputHandler->keyPressed(SDL_SCANCODE_TAB))
	{
		if (!tabKeyPreviouslyPressed)
		{
			show_demo_window = !show_demo_window;
			show_tool_metrics = !show_tool_metrics;
			show_custom_window = !show_custom_window;
		}
		tabKeyPreviouslyPressed = true;
	}
	else
	{
		tabKeyPreviouslyPressed = false;
	}
#endif _DEBUG
	if (Engine::GetInstance()->Paused() == false)
	{
		/*
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);

		//This is the Scenes Player light tracking code or at least part of it
		// assuming Cursour Player and Light Player are the naming schemes.
		Vector2 CursourP = { (float)x, (float)y };
		CursourP *= 1.0f / TbdPixelRenderer->screenScale;
		CursourP += TbdPixelRenderer->GetCameraPosition();

		Vector2 LightP = TbdPixelRenderer->lightSource[0].position;

		//Direction? Difference?
		Vector2 D = LightP - CursourP;
		float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;
		TbdPixelRenderer->lightSource[0].angle = Angle;

		ImageBuffer* playerEntity = TbdPixelRenderer->animatedObjects[0][0];
		Vector2 ScreenHalfSize = 0.5f * Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
		Vector2 BitmapHalfDim = 0.5f * playerEntity->size;
		TbdPixelRenderer->SetCameraPosition(playerEntity->position - ScreenHalfSize + BitmapHalfDim);
		*/

		//move the laser
		/*
		if (TestLaser)
		{
			Vector2 Delta = CursourP - TestLaser->Position;
			float Angle = atan2f(Delta.y, Delta.x);
			TestLaser->Direction = Vector2(cosf(Angle), sinf(Angle));
		}
		*/
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_M))
	{
		if (!mKeyDown)
		{
			if (isMuted)
			{
				AudioManager.ResumeMusic();
				AudioManager.ResumeSFX();
			}
			else
			{
				AudioManager.PauseMusic();
				AudioManager.PauseSFX();
			}
			isMuted = !isMuted;
		}
		mKeyDown = true;
	}
	else
	{
		mKeyDown = false;
	}
}

void TbdTestScene::Update(float dt)
{
	if (CheckGameScenes() || CheckRestart())
		return;

	//eventally have player handle these lights
	//TbdPixelRenderer->lightSource[1].position = TbdPixelRenderer->animatedObjects[0][0]->position + Vector2{ 3,3 };
	//TbdPixelRenderer->lightSource[0].position = TbdPixelRenderer->animatedObjects[0][0]->position + Vector2{ 3,3 };

	Inputs* inputHandler = Inputs::GetInstance();
	AudioManager.Update();
	inputHandler->handleInput();
	bool check = winState;

	if (inputHandler->keyPressed(SDL_SCANCODE_Y))
	{
		winState = (check ? false : true);
	}
	if (LevelBuilder::GetWinState())
	{
		if (dt != 0)
		{
			TbdPixelRenderer->menuBuffer = new ImageBuffer{ SCREEN_SIZE_X,SCREEN_SIZE_Y };
			ImageBuffer* temp = new ImageBuffer{ "./Assets/PPM/TemporaryWinScreen.ppm" };
			TbdPixelRenderer->menuBuffer->AddSprite(temp, Vector2{ -20,-5 });
			Engine::GetInstance()->SetPause(true);
		}
		if (inputHandler->keyPressed(SDL_SCANCODE_RETURN))
		{
			SceneSystem::RestartScene();
		}
	}
	TbdPlayerMovement(dt);
	handleCheatCodes();

	ImGuiInterg();
	TbdPixelRenderer->Update(dt);
	entityManagerTBD->pInput = Inputs::GetInstance();
}

void TbdTestScene::Render()
{
	return;
}

Engine::EngineCode TbdTestScene::Exit()
{
	EntityContainer::GetInstance()->FreeAll();
	LevelBuilder::SetWinState(false);
	LevelBuilder::setDoor(false);
	Inputs::GetInstance()->InputKeyClear();
	TbdPixelRenderer->CleanRenderer();
	winState = false;
	return Engine::NothingBad;
}

Engine::EngineCode TbdTestScene::Unload()
{
	winState = false;
	TbdTestSceneinstance->~Scene();
	TbdTestSceneinstance = nullptr;
	return Engine::NothingBad;
}

Scene* TbdTestSceneGetInstance(void)
{
	static Scene* TbdSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!TbdTestSceneinstance) {
		TbdTestSceneinstance = new TbdTestScene();
	}
	return TbdTestSceneinstance;
}

#ifndef ImGUI_Functions

void TbdTestScene::ImGuiInterg()
{
#ifdef _DEBUG
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (show_custom_window)
	{
		ImGuiWindow();
	}

	ImGui::Render();
#endif
}

void TbdTestScene::ImGuiWindow()
{
#ifdef _DEBUG
	if (show_custom_window)
	{
		ImGui::Begin("TBDTestScene");
		ImGui::BeginMainMenuBar();

		if (ImGui::MenuItem("Reset Scene", NULL, false, true))
			SceneSystem::GetInstance()->RestartScene();
		if (ImGui::MenuItem("Test Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
		if (ImGui::MenuItem("TbdTest Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(TbdTestSceneGetInstance());

		ImGui::EndMainMenuBar();

		ImGui::Text("have fun,");
		ImGui::Text("welcome to the test window!");
		ImGui::Separator();

		if (ImGui::Button("Toggle Metrics/Debug Bar"))
		{
			show_metrics_debug_bar = !show_metrics_debug_bar;
		}

		if (show_metrics_debug_bar)
		{
			ImGui::Text("Metrics/Debugger:");
			ImGui::Separator();
			ImGui::Text("Frame Time: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

			int mouseX = Inputs::GetInstance()->getMouseX();
			int mouseY = Inputs::GetInstance()->getMouseY();
			float screenScale = TbdPixelRenderer->screenScale;
			float cameraPosX = TbdPixelRenderer->GetCameraPosition().x;
			float cameraPosY = TbdPixelRenderer->GetCameraPosition().y;
			float worldMouseX = (mouseX - cameraPosX) / screenScale;
			float worldMouseY = (mouseY - cameraPosY) / screenScale;

			ImGui::Text("World Mouse Position: (%.2f, %.2f)", worldMouseX, worldMouseY);
			ImGui::Text("Local Mouse Position: (%d, %d)", Inputs::GetInstance()->getMouseX(), Inputs::GetInstance()->getMouseY());

			ImGui::Text("Keys Pressed:");
			for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
			{
				if (Inputs::GetInstance()->keyPressed(i))
				{
					const char* keyName = SDL_GetKeyName(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(i)));
					ImGui::BulletText("%s is being pressed", keyName);
				}
			}

			ImGui::SliderFloat("Brightness", &TbdPixelRenderer->minBrightness, 0.0f, 255.0f);
			ImGui::SliderFloat("Saturation", &TbdPixelRenderer->saturationPercent, 0.0f, 1.0f);

			ImGui::Separator();
		}

		ImGui::Separator();

		ImGui::Text("Cheats:");

		if (ImGui::Button("FullBright Cheat:"))
		{
			isGravePressedForCheat = !isGravePressedForCheat;

			cheatFullbright();
		}
		ImGui::SameLine();
		ImGui::Text(isGravePressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Only Lights Cheat:"))
		{
			isQPressedForCheat = !isQPressedForCheat;

			cheatOnlyLights();
		}
		ImGui::SameLine();
		ImGui::Text(isQPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("NormalMap Cheat:"))
		{
			isNPressedForCheat = !isNPressedForCheat;

			cheatNormalMap();
		}
		ImGui::SameLine();
		ImGui::Text(isNPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Wall Hitboxes Cheat:"))
		{
			isCPressedForCheat = !isCPressedForCheat;

			cheatWallHitboxes();
		}
		ImGui::SameLine();
		ImGui::Text(isCPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("ScanLines Cheat:"))
		{
			isFPressedForCheat = !isFPressedForCheat;

			cheatScanlines();
		}

		ImGui::SameLine();
		ImGui::Text(isFPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Blur Cheat:"))
		{
			isEPressedForCheat = !isEPressedForCheat;

			cheatBlur();
		}

		ImGui::SameLine();
		ImGui::Text(isEPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Justa Fog Cheat:"))
		{
			isHPressedForCheat = !isHPressedForCheat;

			cheatOnlyFog();
		}

		ImGui::SameLine();
		ImGui::Text(isHPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Fog Cheat:"))
		{
			isGPressedForCheat = !isGPressedForCheat;

			cheatFog();
		}

		ImGui::SameLine();
		ImGui::Text(isGPressedForCheat ? "Active" : "Inactive");

		ImGui::Separator();

		ImGui::Text("Entities:");
		EntityContainer* entityContainer = EntityContainer::GetInstance();
		int numEntities = entityContainer->CountEntities();

		if (ImGui::TreeNode("All Entities"))
		{
			for (int i = 0; i < numEntities; i++)
			{
				Entity* entity = (*entityContainer)[i];
				if (entity)
				{
					if (ImGui::TreeNode(("Entity %s", entity->GetRealName().c_str())))
					{
						ImGui::Text("Name: %s", entity->GetRealName().c_str());
						ImGui::Text("Entity Number: %d", i);

						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}

		// this does nothing will need to keep working on it
		entityManagerTBD->ShowEntityInfo();

		ImGui::Separator();

		ImGui::Text("Particle System:");

		Particle** particles = TbdPixelRenderer->particleManager->GetParticles();

		if (ImGui::TreeNode("All Active Particles"))
		{
			for (int i = 0; i < MAX_PARTICLES; ++i)
			{
				if (particles[i] != nullptr)
				{
					if (ImGui::TreeNode(("Particle " + std::to_string(i)).c_str()))
					{
						ImGui::Text("Particle Type: %d", particles[i]->particleType);
						ImGui::Text("Position: (%.2f, %.2f)", particles[i]->position.x, particles[i]->position.y);
						ImGui::Text("Direction: (%.2f, %.2f)", particles[i]->direction.x, particles[i]->direction.y);
						ImGui::Text("Speed: (%.2f, %.2f)", particles[i]->speed.x, particles[i]->speed.y);
						ImGui::Text("Color: (%d, %d, %d, %d)", particles[i]->color.r, particles[i]->color.g, particles[i]->color.b, particles[i]->color.a);

						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		ImGui::Text("Scene Tools:");

		ImGui::End();
	}
#endif
}
#endif