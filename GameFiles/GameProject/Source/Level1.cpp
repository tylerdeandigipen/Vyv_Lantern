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

#include "Level1.h"
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

Logging& Level1Logger = Logging::GetInstance("debugLog.log");

SDL_Renderer* Level1Renderer_sdl;
Renderer* Level1Renderer = Renderer::GetInstance();

SDL_Window* Level1Window;

SDL_GLContext Level1GlContext;

Scene* Level1instance = NULL;

laser_emitter* Level1WLaser;

constexpr const char* _OBJECT_LIST = "./Data/Scenes/Level1Final/Level1FinalOBJECTS.json";
constexpr const char* _SCENE = "./Data/Scenes/Level1Final/Level1Final.json";

Level1::Level1() : Scene("Level1")
{
	entityManagerTBD = std::make_unique<EntityManager>();
}

Engine::EngineCode Level1::Load()
{
	AudioManager.LoadMusicFromJSON("./Data/music.json");//line is good
	AudioManager.LoadSFXFromJSON("./Data/SFX.json");// line is goodplay

	if (entityManagerTBD->InitializeProperties(_OBJECT_LIST))
		std::cout << "Property load success!\n";

	return Engine::NothingBad;
}

Engine::EngineCode Level1::Init()
{
	Inputs::GetInstance()->SetWindow(Level1Window);

	//exporttests
	//FileIO::GetInstance()->ExportTileMap("export_tests");

	Light tempLight;
	Light tempLight2;

	// Create SDL Window
	Level1Window = PlatformSystem::GetInstance()->GetWindowHandle();
	Level1Renderer->window = Level1Window;

	//initialize level data
	//EntityContainer::GetInstance()->ReadEntities();
	LevelBuilder::GetInstance()->LoadTileMap(_SCENE);

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
		Level1Renderer->particleManager->AddParticle(testParticle);
	}

	//AudioManager.PlayMusic("drips"); //line is good

	//AudioManager.PlayMusic("forest"); //line is good
	AudioManager.PlayMusic("bgm");
	AudioManager.DecreaseMusicVolume(0.7f);

	AudioManager.PlaySFX("bgAmbience", 0.75f);

	AudioManager.PlaySFX("laserAmbience", 0.2f);

	FontSystem fontSystem;

	fontSystem.init("Font/MouldyCheeseRegular-WyMWG.ttf", 10);

	entityManagerTBD->pRenderer = Level1Renderer;

	Engine::GetInstance()->SetPause(false);
	return Engine::NothingBad;
}

#ifndef Render_Toggle_Functions

namespace Name_Level1
{
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

	bool isMuted = false;
	bool mKeyDown = false;

	void TbdPlayerMovement(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		Inputs* inputHandler = Inputs::GetInstance();

		if (inputHandler->keyPressed(SDL_SCANCODE_TAB))
		{
			if (!Name_Level1::tabKeyPreviouslyPressed)
			{
				Name_Level1::show_demo_window = !Name_Level1::show_demo_window;
				Name_Level1::show_tool_metrics = !Name_Level1::show_tool_metrics;
				Name_Level1::show_custom_window = !Name_Level1::show_custom_window;
			}
			Name_Level1::tabKeyPreviouslyPressed = true;
		}
		else
		{
			Name_Level1::tabKeyPreviouslyPressed = false;
		}

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
			if (!Name_Level1::mKeyDown)
			{
				if (Name_Level1::isMuted)
				{
					AudioManager.ResumeMusic();
					AudioManager.ResumeSFX();
				}
				else
				{
					AudioManager.PauseMusic();
					AudioManager.PauseSFX();
				}
				Name_Level1::isMuted = !Name_Level1::isMuted;
			}
			Name_Level1::mKeyDown = true;
		}
		else
		{
			Name_Level1::mKeyDown = false;
		}
	}
}

void Level1::cheatFullbright()
{
	if (Level1Renderer->isFullbright == false)
		Level1Renderer->isFullbright = true;
	else
		Level1Renderer->isFullbright = false;
	Name_Level1::TbdCanToggleFullBright = false;
}

void Level1::cheatBlur()
{
	if (Level1Renderer->doBlur == false)
		Level1Renderer->doBlur = true;
	else
		Level1Renderer->doBlur = false;
	Name_Level1::TbdCanPlaceLight = 0;
}

void Level1::cheatOnlyLights()
{
	if (Level1Renderer->renderOnlyLights == false)
		Level1Renderer->renderOnlyLights = true;
	else
		Level1Renderer->renderOnlyLights = false;
	Name_Level1::TbdCanToggleOnlyLights = false;
}

void Level1::cheatNormalMap()
{
	if (Level1Renderer->renderNormalMap == false)
		Level1Renderer->renderNormalMap = true;
	else
		Level1Renderer->renderNormalMap = false;
	Name_Level1::TbdCanToggleNormalDisplay = false;
}

void Level1::cheatWallHitboxes()
{
	if (Level1Renderer->renderWallHitboxes == false)
		Level1Renderer->renderWallHitboxes = true;
	else
		Level1Renderer->renderWallHitboxes = false;
	Name_Level1::TbdCanRenderWallHitboxes = 0;
}

void Level1::cheatScanlines()
{
	if (Level1Renderer->doScanLines == false)
		Level1Renderer->doScanLines = true;
	else
		Level1Renderer->doScanLines = false;
	Name_Level1::canToggleScanLines = 0;
}

void Level1::cheatOnlyFog()
{
	if (Level1Renderer->drawRawFog == false)
		Level1Renderer->drawRawFog = true;
	else
		Level1Renderer->drawRawFog = false;
	Name_Level1::canRenderRawFog = 0;
}

void Level1::cheatFog()
{
	if (Level1Renderer->doFog == false)
		Level1Renderer->doFog = true;
	else
		Level1Renderer->doFog = false;
	Name_Level1::canToggleFog = 0;
}

void Level1::handleCheatCodes()
{
	Inputs* inputHandler = Inputs::GetInstance();

	if (inputHandler->keyPressed(SDL_SCANCODE_ESCAPE))
	{
		if (Name_Level1::CanPause == true)
		{
			//AudioManager.PlaySFX("creak");
			if (Engine::GetInstance()->Paused() == false)
				Engine::GetInstance()->SetPause(true);
			else
				Engine::GetInstance()->SetPause(false);
		}
		Name_Level1::CanPause = false;
	}
	else
	{
		Name_Level1::CanPause = true;
	}

#ifdef _DEBUG

	if (inputHandler->keyPressed(SDL_SCANCODE_GRAVE) && Name_Level1::TbdCanToggleFullBright == true)
	{
		cheatFullbright();

		if (!Name_Level1::isGravePressedForCheat)
		{
			Name_Level1::isGravePressedForCheat = true;
		}
		else
		{
			Name_Level1::isGravePressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_GRAVE))
	{
		Name_Level1::TbdCanToggleFullBright = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_Q) && Name_Level1::TbdCanToggleOnlyLights == true)
	{
		cheatOnlyLights();

		if (!Name_Level1::isQPressedForCheat)
		{
			Name_Level1::isQPressedForCheat = true;
		}
		else
		{
			Name_Level1::isQPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_Q))
	{
		Name_Level1::TbdCanToggleOnlyLights = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_N) && Name_Level1::TbdCanToggleNormalDisplay == true)
	{
		cheatNormalMap();

		if (!Name_Level1::isNPressedForCheat)
		{
			Name_Level1::isNPressedForCheat = true;
		}
		else
		{
			Name_Level1::isNPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_N))
	{
		Name_Level1::TbdCanToggleNormalDisplay = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_C) && Name_Level1::TbdCanRenderWallHitboxes == 1)
	{
		cheatWallHitboxes();

		if (!Name_Level1::isCPressedForCheat)
		{
			Name_Level1::isCPressedForCheat = true;
		}
		else
		{
			Name_Level1::isCPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_C))
	{
		Name_Level1::TbdCanRenderWallHitboxes = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_F) && Name_Level1::canToggleScanLines == 1)
	{
		cheatScanlines();

		if (!Name_Level1::isFPressedForCheat)
		{
			Name_Level1::isFPressedForCheat = true;
		}
		else
		{
			Name_Level1::isFPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_F))
	{
		Name_Level1::canToggleScanLines = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_H) && Name_Level1::canRenderRawFog == 1)
	{
		cheatOnlyFog();

		if (!Name_Level1::isHPressedForCheat)
		{
			Name_Level1::isHPressedForCheat = true;
		}
		else
		{
			Name_Level1::isHPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_H))
	{
		Name_Level1::canRenderRawFog = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_G) && Name_Level1::canToggleFog == 1)
	{
		cheatFog();

		if (!Name_Level1::isGPressedForCheat)
		{
			Name_Level1::isGPressedForCheat = true;
		}
		else
		{
			Name_Level1::isGPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_G))
	{
		Name_Level1::canToggleFog = 1;
	}
#endif _DEBUG
}

#endif

void Level1::Update(float dt)
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
			Level1Renderer->menuBuffer = new ImageBuffer{ SCREEN_SIZE_X,SCREEN_SIZE_Y };
			ImageBuffer* temp = new ImageBuffer{ "./Assets/PPM/TemporaryWinScreen.ppm" };
			Level1Renderer->menuBuffer->AddSprite(temp, Vector2{ -20,-5 });
			Engine::GetInstance()->SetPause(true);
		}
		if (inputHandler->keyPressed(SDL_SCANCODE_RETURN))
		{
			SceneSystem::RestartScene();
		}
	}
	Name_Level1::TbdPlayerMovement(dt);
	handleCheatCodes();

	ImGuiInterg();
	Level1Renderer->Update(dt);
	entityManagerTBD->pInput = Inputs::GetInstance();
}

void Level1::Render()
{
	return;
}

Engine::EngineCode Level1::Exit()
{
	EntityContainer::GetInstance()->FreeAll();
	LevelBuilder::SetWinState(false);
	LevelBuilder::setDoor(false);
	Inputs::GetInstance()->InputKeyClear();
	Level1Renderer->CleanRenderer();
	winState = false;
	return Engine::NothingBad;
}

Engine::EngineCode Level1::Unload()
{
	winState = false;
	Level1instance->~Scene();
	Level1Renderer = nullptr;
	return Engine::NothingBad;
}

Scene* Level1GetInstance(void)
{
	static Scene* TbdSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!Level1instance)
	{
		Level1instance = new Level1();
	}
	return Level1instance;
}

#ifndef ImGUI_Functions

void Level1::ImGuiInterg()
{
#ifdef _DEBUG
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (Name_Level1::show_custom_window)
	{
		ImGuiWindow();
	}

	ImGui::Render();
#endif
}

void Level1::ImGuiWindow()
{
#ifdef _DEBUG
	if (Name_Level1::show_custom_window)
	{
		ImGui::Begin("TBDTestScene");
		ImGui::BeginMainMenuBar();

		if (ImGui::MenuItem("Reset Scene", NULL, false, true))
			SceneSystem::GetInstance()->RestartScene();
		if (ImGui::MenuItem("Test Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
		if (ImGui::MenuItem("TbdTest Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(Level1GetInstance());

		ImGui::EndMainMenuBar();

		ImGui::Text("have fun,");
		ImGui::Text("welcome to the test window!");
		ImGui::Separator();

		if (ImGui::Button("Toggle Metrics/Debug Bar"))
		{
			Name_Level1::show_metrics_debug_bar = !Name_Level1::show_metrics_debug_bar;
		}

		if (Name_Level1::show_metrics_debug_bar)
		{
			ImGui::Text("Metrics/Debugger:");
			ImGui::Separator();
			ImGui::Text("Frame Time: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

			int mouseX = Inputs::GetInstance()->getMouseX();
			int mouseY = Inputs::GetInstance()->getMouseY();
			float screenScale = Level1Renderer->screenScale;
			float cameraPosX = Level1Renderer->GetCameraPosition().x;
			float cameraPosY = Level1Renderer->GetCameraPosition().y;
			float worldMouseX = (mouseX - cameraPosX) / screenScale;
			float worldMouseY = (mouseY - cameraPosY) / screenScale;

			ImGui::Text("Mouse Position: (%.2f, %.2f)", worldMouseX, worldMouseY);

			ImGui::Text("Keys Pressed:");
			for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
			{
				if (Inputs::GetInstance()->keyPressed(i))
				{
					const char* keyName = SDL_GetKeyName(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(i)));
					ImGui::BulletText("%s is being pressed", keyName);
				}
			}

			ImGui::SliderFloat("Brightness", &Level1Renderer->minBrightness, 0.0f, 255.0f);
			ImGui::SliderFloat("Saturation", &Level1Renderer->saturationPercent, 0.0f, 1.0f);

			ImGui::Separator();
		}

		ImGui::Separator();

		ImGui::Text("Cheats:");

		if (ImGui::Button("FullBright Cheat:"))
		{
			Name_Level1::isGravePressedForCheat = !Name_Level1::isGravePressedForCheat;

			cheatFullbright();
		}
		ImGui::SameLine();
		ImGui::Text(Name_Level1::isGravePressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Only Lights Cheat:"))
		{
			Name_Level1::isQPressedForCheat = !Name_Level1::isQPressedForCheat;

			cheatOnlyLights();
		}
		ImGui::SameLine();
		ImGui::Text(Name_Level1::isQPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("NormalMap Cheat:"))
		{
			Name_Level1::isNPressedForCheat = !Name_Level1::isNPressedForCheat;

			cheatNormalMap();
		}
		ImGui::SameLine();
		ImGui::Text(Name_Level1::isNPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Wall Hitboxes Cheat:"))
		{
			Name_Level1::isCPressedForCheat = !Name_Level1::isCPressedForCheat;

			cheatWallHitboxes();
		}
		ImGui::SameLine();
		ImGui::Text(Name_Level1::isCPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("ScanLines Cheat:"))
		{
			Name_Level1::isFPressedForCheat = !Name_Level1::isFPressedForCheat;

			cheatScanlines();
		}

		ImGui::SameLine();
		ImGui::Text(Name_Level1::isFPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Blur Cheat:"))
		{
			Name_Level1::isEPressedForCheat = !Name_Level1::isEPressedForCheat;

			cheatBlur();
		}

		ImGui::SameLine();
		ImGui::Text(Name_Level1::isEPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Justa Fog Cheat:"))
		{
			Name_Level1::isHPressedForCheat = !Name_Level1::isHPressedForCheat;

			cheatOnlyFog();
		}

		ImGui::SameLine();
		ImGui::Text(Name_Level1::isHPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Fog Cheat:"))
		{
			Name_Level1::isGPressedForCheat = !Name_Level1::isGPressedForCheat;

			cheatFog();
		}

		ImGui::SameLine();
		ImGui::Text(Name_Level1::isGPressedForCheat ? "Active" : "Inactive");

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

		Particle** particles = Level1Renderer->particleManager->GetParticles();

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