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

#include "Section1Final.h"
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

Logging& Section1FinalLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* Section1FinalRenderer_sdl;
Renderer* Section1FinalRenderer = Renderer::GetInstance();

SDL_Window* Section1FinalWindow;

SDL_GLContext Section1FinalGlContext;

Scene* Section1Finalinstance = NULL;

laser_emitter* Section1FinalWLaser;

Section1Final::Section1Final() : Scene("Section1Final")
{
	entityManagerTBD = std::make_unique<EntityManager>();
}

Engine::EngineCode Section1Final::Load()
{
	AudioManager.LoadMusicFromJSON("./Data/music.json");//line is good
	AudioManager.LoadSFXFromJSON("./Data/SFX.json");// line is goodplay

	if (entityManagerTBD->InitializeProperties("./Data/Scenes/Section1FinalNot1FPS/Section1FinalNot1FPSOBJECTS.json"))
		std::cout << "Property load success!\n";

	return Engine::NothingBad;
}

Engine::EngineCode Section1Final::Init()
{
	Inputs::GetInstance()->SetWindow(Section1FinalWindow);

	//exporttests
	//FileIO::GetInstance()->ExportTileMap("export_tests");

	Light tempLight;
	Light tempLight2;

	// Create SDL Window
	Section1FinalWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	Section1FinalRenderer->window = Section1FinalWindow;

	//initialize level data
	//EntityContainer::GetInstance()->ReadEntities();
	LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/Section1FinalNot1FPS/Section1FinalNot1FPS.json");

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
		Section1FinalRenderer->particleManager->AddParticle(testParticle);
	}

	//AudioManager.PlayMusic("drips"); //line is good

	//AudioManager.PlayMusic("forest"); //line is good
	AudioManager.PlayMusic("bgm");
	AudioManager.DecreaseMusicVolume(0.7f);

	AudioManager.PlaySFX("bgAmbience");
	AudioManager.PlaySFX("laserAmbience");

	FontSystem fontSystem;

	fontSystem.init("Font/MouldyCheeseRegular-WyMWG.ttf", 10);

	entityManagerTBD->pRenderer = Section1FinalRenderer;

	Engine::GetInstance()->SetPause(false);
	return Engine::NothingBad;
}

#ifndef Render_Toggle_Functions

namespace Name_Section1Final
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
			if (!Name_Section1Final::tabKeyPreviouslyPressed)
			{
				Name_Section1Final::show_demo_window = !Name_Section1Final::show_demo_window;
				Name_Section1Final::show_tool_metrics = !Name_Section1Final::show_tool_metrics;
				Name_Section1Final::show_custom_window = !Name_Section1Final::show_custom_window;
			}
			Name_Section1Final::tabKeyPreviouslyPressed = true;
		}
		else
		{
			Name_Section1Final::tabKeyPreviouslyPressed = false;
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
			if (!Name_Section1Final::mKeyDown)
			{
				if (Name_Section1Final::isMuted)
				{
					AudioManager.ResumeMusic();
					AudioManager.ResumeSFX();
				}
				else
				{
					AudioManager.PauseMusic();
					AudioManager.PauseSFX();
				}
				Name_Section1Final::isMuted = !Name_Section1Final::isMuted;
			}
			Name_Section1Final::mKeyDown = true;
		}
		else
		{
			Name_Section1Final::mKeyDown = false;
		}
	}
}

void Section1Final::cheatFullbright()
{
	if (Section1FinalRenderer->isFullbright == false)
		Section1FinalRenderer->isFullbright = true;
	else
		Section1FinalRenderer->isFullbright = false;
	Name_Section1Final::TbdCanToggleFullBright = false;
}

void Section1Final::cheatBlur()
{
	if (Section1FinalRenderer->doBlur == false)
		Section1FinalRenderer->doBlur = true;
	else
		Section1FinalRenderer->doBlur = false;
	Name_Section1Final::TbdCanPlaceLight = 0;
}

void Section1Final::cheatOnlyLights()
{
	if (Section1FinalRenderer->renderOnlyLights == false)
		Section1FinalRenderer->renderOnlyLights = true;
	else
		Section1FinalRenderer->renderOnlyLights = false;
	Name_Section1Final::TbdCanToggleOnlyLights = false;
}

void Section1Final::cheatNormalMap()
{
	if (Section1FinalRenderer->renderNormalMap == false)
		Section1FinalRenderer->renderNormalMap = true;
	else
		Section1FinalRenderer->renderNormalMap = false;
	Name_Section1Final::TbdCanToggleNormalDisplay = false;
}

void Section1Final::cheatWallHitboxes()
{
	if (Section1FinalRenderer->renderWallHitboxes == false)
		Section1FinalRenderer->renderWallHitboxes = true;
	else
		Section1FinalRenderer->renderWallHitboxes = false;
	Name_Section1Final::TbdCanRenderWallHitboxes = 0;
}

void Section1Final::cheatScanlines()
{
	if (Section1FinalRenderer->doScanLines == false)
		Section1FinalRenderer->doScanLines = true;
	else
		Section1FinalRenderer->doScanLines = false;
	Name_Section1Final::canToggleScanLines = 0;
}

void Section1Final::cheatOnlyFog()
{
	if (Section1FinalRenderer->drawRawFog == false)
		Section1FinalRenderer->drawRawFog = true;
	else
		Section1FinalRenderer->drawRawFog = false;
	Name_Section1Final::canRenderRawFog = 0;
}

void Section1Final::cheatFog()
{
	if (Section1FinalRenderer->doFog == false)
		Section1FinalRenderer->doFog = true;
	else
		Section1FinalRenderer->doFog = false;
	Name_Section1Final::canToggleFog = 0;
}

void Section1Final::handleCheatCodes()
{
	Inputs* inputHandler = Inputs::GetInstance();

	if (inputHandler->keyPressed(SDL_SCANCODE_ESCAPE))
	{
		if (Name_Section1Final::CanPause == true)
		{
			AudioManager.PlaySFX("creak");
			if (Engine::GetInstance()->Paused() == false)
				Engine::GetInstance()->SetPause(true);
			else
				Engine::GetInstance()->SetPause(false);
		}
		Name_Section1Final::CanPause = false;
	}
	else
	{
		Name_Section1Final::CanPause = true;
	}
	
#ifdef _DEBUG

	if (inputHandler->keyPressed(SDL_SCANCODE_GRAVE) && Name_Section1Final::TbdCanToggleFullBright == true)
	{
		cheatFullbright();

		if (!Name_Section1Final::isGravePressedForCheat)
		{
			Name_Section1Final::isGravePressedForCheat = true;
		}
		else
		{
			Name_Section1Final::isGravePressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_GRAVE))
	{
		Name_Section1Final::TbdCanToggleFullBright = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_Q) && Name_Section1Final::TbdCanToggleOnlyLights == true)
	{
		cheatOnlyLights();

		if (!Name_Section1Final::isQPressedForCheat)
		{
			Name_Section1Final::isQPressedForCheat = true;
		}
		else
		{
			Name_Section1Final::isQPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_Q))
	{
		Name_Section1Final::TbdCanToggleOnlyLights = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_N) && Name_Section1Final::TbdCanToggleNormalDisplay == true)
	{
		cheatNormalMap();

		if (!Name_Section1Final::isNPressedForCheat)
		{
			Name_Section1Final::isNPressedForCheat = true;
		}
		else
		{
			Name_Section1Final::isNPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_N))
	{
		Name_Section1Final::TbdCanToggleNormalDisplay = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_C) && Name_Section1Final::TbdCanRenderWallHitboxes == 1)
	{
		cheatWallHitboxes();

		if (!Name_Section1Final::isCPressedForCheat)
		{
			Name_Section1Final::isCPressedForCheat = true;
		}
		else
		{
			Name_Section1Final::isCPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_C))
	{
		Name_Section1Final::TbdCanRenderWallHitboxes = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_F) && Name_Section1Final::canToggleScanLines == 1)
	{
		cheatScanlines();

		if (!Name_Section1Final::isFPressedForCheat)
		{
			Name_Section1Final::isFPressedForCheat = true;
		}
		else
		{
			Name_Section1Final::isFPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_F))
	{
		Name_Section1Final::canToggleScanLines = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_H) && Name_Section1Final::canRenderRawFog == 1)
	{
		cheatOnlyFog();

		if (!Name_Section1Final::isHPressedForCheat)
		{
			Name_Section1Final::isHPressedForCheat = true;
		}
		else
		{
			Name_Section1Final::isHPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_H))
	{
		Name_Section1Final::canRenderRawFog = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_G) && Name_Section1Final::canToggleFog == 1)
	{
		cheatFog();

		if (!Name_Section1Final::isGPressedForCheat)
		{
			Name_Section1Final::isGPressedForCheat = true;
		}
		else
		{
			Name_Section1Final::isGPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_G))
	{
		Name_Section1Final::canToggleFog = 1;
	}
#endif _DEBUG
}

#endif




void Section1Final::Update(float dt)
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
			Section1FinalRenderer->menuBuffer = new ImageBuffer{ SCREEN_SIZE_X,SCREEN_SIZE_Y };
			ImageBuffer* temp = new ImageBuffer{ "./Assets/PPM/TemporaryWinScreen.ppm" };
			Section1FinalRenderer->menuBuffer->AddSprite(temp, Vector2{ -20,-5 });
			Engine::GetInstance()->SetPause(true);
		}
		if (inputHandler->keyPressed(SDL_SCANCODE_RETURN))
		{
			SceneSystem::RestartScene();
		}
	}
	Name_Section1Final::TbdPlayerMovement(dt);
	handleCheatCodes();

	ImGuiInterg();
	Section1FinalRenderer->Update(dt);
	entityManagerTBD->pInput = Inputs::GetInstance();
}

void Section1Final::Render()
{
	return;
}

Engine::EngineCode Section1Final::Exit()
{
	EntityContainer::GetInstance()->FreeAll();
	LevelBuilder::SetWinState(false);
	LevelBuilder::setDoor(false);
	Inputs::GetInstance()->InputKeyClear();
	Section1FinalRenderer->CleanRenderer();
	winState = false;
	return Engine::NothingBad;
}

Engine::EngineCode Section1Final::Unload()
{
	winState = false;
	Section1Finalinstance->~Scene();
	Section1FinalRenderer = nullptr;
	return Engine::NothingBad;
}

Scene* Section1FinalGetInstance(void)
{
	static Scene* TbdSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!Section1Finalinstance) {
		Section1Finalinstance = new Section1Final();
	}
	return Section1Finalinstance;
}

#ifndef ImGUI_Functions

void Section1Final::ImGuiInterg()
{
#ifdef _DEBUG
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (Name_Section1Final::show_custom_window)
	{
		ImGuiWindow();
	}

	ImGui::Render();
#endif
}

void Section1Final::ImGuiWindow()
{
#ifdef _DEBUG
	if (Name_Section1Final::show_custom_window)
	{
		ImGui::Begin("TBDTestScene");
		ImGui::BeginMainMenuBar();

		if (ImGui::MenuItem("Reset Scene", NULL, false, true))
			SceneSystem::GetInstance()->RestartScene();
		if (ImGui::MenuItem("Test Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
		if (ImGui::MenuItem("TbdTest Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(Section1FinalGetInstance());

		ImGui::EndMainMenuBar();

		ImGui::Text("have fun,");
		ImGui::Text("welcome to the test window!");
		ImGui::Separator();

		if (ImGui::Button("Toggle Metrics/Debug Bar"))
		{
			Name_Section1Final::show_metrics_debug_bar = !Name_Section1Final::show_metrics_debug_bar;
		}

		if (Name_Section1Final::show_metrics_debug_bar)
		{
			ImGui::Text("Metrics/Debugger:");
			ImGui::Separator();
			ImGui::Text("Frame Time: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

			int mouseX = Inputs::GetInstance()->getMouseX();
			int mouseY = Inputs::GetInstance()->getMouseY();
			float screenScale = Section1FinalRenderer->screenScale;
			float cameraPosX = Section1FinalRenderer->GetCameraPosition().x;
			float cameraPosY = Section1FinalRenderer->GetCameraPosition().y;
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

			ImGui::SliderFloat("Brightness", &Section1FinalRenderer->minBrightness, 0.0f, 255.0f);
			ImGui::SliderFloat("Saturation", &Section1FinalRenderer->saturationPercent, 0.0f, 1.0f);

			ImGui::Separator();
		}

		ImGui::Separator();

		ImGui::Text("Cheats:");

		if (ImGui::Button("FullBright Cheat:"))
		{
			Name_Section1Final::isGravePressedForCheat = !Name_Section1Final::isGravePressedForCheat;

			cheatFullbright();
		}
		ImGui::SameLine();
		ImGui::Text(Name_Section1Final::isGravePressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Only Lights Cheat:"))
		{
			Name_Section1Final::isQPressedForCheat = !Name_Section1Final::isQPressedForCheat;

			cheatOnlyLights();
		}
		ImGui::SameLine();
		ImGui::Text(Name_Section1Final::isQPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("NormalMap Cheat:"))
		{
			Name_Section1Final::isNPressedForCheat = !Name_Section1Final::isNPressedForCheat;

			cheatNormalMap();
		}
		ImGui::SameLine();
		ImGui::Text(Name_Section1Final::isNPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Wall Hitboxes Cheat:"))
		{
			Name_Section1Final::isCPressedForCheat = !Name_Section1Final::isCPressedForCheat;

			cheatWallHitboxes();
		}
		ImGui::SameLine();
		ImGui::Text(Name_Section1Final::isCPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("ScanLines Cheat:"))
		{
			Name_Section1Final::isFPressedForCheat = !Name_Section1Final::isFPressedForCheat;

			cheatScanlines();
		}

		ImGui::SameLine();
		ImGui::Text(Name_Section1Final::isFPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Blur Cheat:"))
		{
			Name_Section1Final::isEPressedForCheat = !Name_Section1Final::isEPressedForCheat;

			cheatBlur();
		}

		ImGui::SameLine();
		ImGui::Text(Name_Section1Final::isEPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Justa Fog Cheat:"))
		{
			Name_Section1Final::isHPressedForCheat = !Name_Section1Final::isHPressedForCheat;

			cheatOnlyFog();
		}

		ImGui::SameLine();
		ImGui::Text(Name_Section1Final::isHPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Fog Cheat:"))
		{
			Name_Section1Final::isGPressedForCheat = !Name_Section1Final::isGPressedForCheat;

			cheatFog();
		}

		ImGui::SameLine();
		ImGui::Text(Name_Section1Final::isGPressedForCheat ? "Active" : "Inactive");

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

		Particle** particles = Section1FinalRenderer->particleManager->GetParticles();

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