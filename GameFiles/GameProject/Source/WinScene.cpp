//------------------------------------------------------------------------------
//
// File Name:	WinScene.cpp
// Author(s):	Michael Howard
//
// Purpose:     Resolutions and Conclusion
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

#include "WinScene.h"
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

Logging& WinSceneLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* WinSceneRenderer_sdl;
Renderer* WinSceneRenderer = Renderer::GetInstance();

SDL_Window* WinSceneWindow;

SDL_GLContext WinSceneGlContext;

Scene* WinSceneinstance = NULL;

laser_emitter* WinSceneWLaser;

constexpr const char* _OBJECT_LIST = "./Data/Scenes/Resolution/Resolution.json";
constexpr const char* _SCENE = "./Data/Scenes/Resolution/Resolution.json";

WinScene::WinScene() : Scene("WinScene")
{
	entityManagerTBD = std::make_unique<EntityManager>();
}

Engine::EngineCode WinScene::Load()
{
	AudioManager.LoadMusicFromJSON("./Data/music.json");//line is good
	AudioManager.LoadSFXFromJSON("./Data/SFX.json");// line is goodplay

	if (entityManagerTBD->InitializeProperties(_OBJECT_LIST))
		std::cout << "Property load success!\n";

	return Engine::NothingBad;
}

Engine::EngineCode WinScene::Init()
{
	//exporttests
	//FileIO::GetInstance()->ExportTileMap("export_tests");

	Light tempLight;
	Light tempLight2;

	// Create SDL Window
	WinSceneRenderer = Renderer::GetInstance();
	WinSceneWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	WinSceneRenderer->window = WinSceneWindow;
	Inputs::GetInstance()->SetWindow(WinSceneWindow);
	WinSceneRenderer->SetCameraPosition({ -16.0f, 220.0f });
	WinSceneRenderer->minBrightness = 175.0f;

	//initialize level data
	//EntityContainer::GetInstance()->ReadEntities();
	LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/Resolution/Resolution.json");

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
		WinSceneRenderer->particleManager->AddParticle(testParticle);
	}

	//AudioManager.PlayMusic("drips"); //line is good

	//AudioManager.PlayMusic("forest"); //line is good
	//AudioManager.PlayMusic("bgm");
	//AudioManager.DecreaseMusicVolume(0.7f);

	//AudioManager.PlaySFX("bgAmbience", 0.75);

	//AudioManager.PlaySFX("laserAmbience", 0.2f);

	FontSystem fontSystem;

	fontSystem.init("Font/MouldyCheeseRegular-WyMWG.ttf", 10);

	entityManagerTBD->pRenderer = WinSceneRenderer;

	Engine::GetInstance()->SetPause(false);
	return Engine::NothingBad;
}

namespace Name_WinScene
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
			if (!Name_WinScene::tabKeyPreviouslyPressed)
			{
				Name_WinScene::show_demo_window = !Name_WinScene::show_demo_window;
				Name_WinScene::show_tool_metrics = !Name_WinScene::show_tool_metrics;
				Name_WinScene::show_custom_window = !Name_WinScene::show_custom_window;
			}
			Name_WinScene::tabKeyPreviouslyPressed = true;
		}
		else
		{
			Name_WinScene::tabKeyPreviouslyPressed = false;
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

#ifdef _DEBUG

		if (inputHandler->keyPressed(SDL_SCANCODE_M))
		{
			if (!Name_WinScene::mKeyDown)
			{
				if (Name_WinScene::isMuted)
				{
					AudioManager.ResumeMusic();
					AudioManager.ResumeSFX();
				}
				else
				{
					AudioManager.PauseMusic();
					AudioManager.PauseSFX();
				}
				Name_WinScene::isMuted = !Name_WinScene::isMuted;
			}
			Name_WinScene::mKeyDown = true;
		}
		else
		{
			Name_WinScene::mKeyDown = false;
		}
#endif // DEBUG
	}
}

void WinScene::cheatFullbright()
{
	if (WinSceneRenderer->isFullbright == false)
		WinSceneRenderer->isFullbright = true;
	else
		WinSceneRenderer->isFullbright = false;
	Name_WinScene::TbdCanToggleFullBright = false;
}

void WinScene::cheatBlur()
{
	if (WinSceneRenderer->doBlur == false)
		WinSceneRenderer->doBlur = true;
	else
		WinSceneRenderer->doBlur = false;
	Name_WinScene::TbdCanPlaceLight = 0;
}

void WinScene::cheatOnlyLights()
{
	if (WinSceneRenderer->renderOnlyLights == false)
		WinSceneRenderer->renderOnlyLights = true;
	else
		WinSceneRenderer->renderOnlyLights = false;
	Name_WinScene::TbdCanToggleOnlyLights = false;
}

void WinScene::cheatNormalMap()
{
	if (WinSceneRenderer->renderNormalMap == false)
		WinSceneRenderer->renderNormalMap = true;
	else
		WinSceneRenderer->renderNormalMap = false;
	Name_WinScene::TbdCanToggleNormalDisplay = false;
}

void WinScene::cheatWallHitboxes()
{
	if (WinSceneRenderer->renderWallHitboxes == false)
		WinSceneRenderer->renderWallHitboxes = true;
	else
		WinSceneRenderer->renderWallHitboxes = false;
	Name_WinScene::TbdCanRenderWallHitboxes = 0;
}

void WinScene::cheatScanlines()
{
	if (WinSceneRenderer->doScanLines == false)
		WinSceneRenderer->doScanLines = true;
	else
		WinSceneRenderer->doScanLines = false;
	Name_WinScene::canToggleScanLines = 0;
}

void WinScene::cheatOnlyFog()
{
	if (WinSceneRenderer->drawRawFog == false)
		WinSceneRenderer->drawRawFog = true;
	else
		WinSceneRenderer->drawRawFog = false;
	Name_WinScene::canRenderRawFog = 0;
}

void WinScene::cheatFog()
{
	if (WinSceneRenderer->doFog == false)
		WinSceneRenderer->doFog = true;
	else
		WinSceneRenderer->doFog = false;
	Name_WinScene::canToggleFog = 0;
}

void WinScene::handleCheatCodes()
{
	Inputs* inputHandler = Inputs::GetInstance();

	if (inputHandler->keyPressed(SDL_SCANCODE_ESCAPE))
	{
		if (Name_WinScene::CanPause == true)
		{
			//AudioManager.PlaySFX("creak");
			if (Engine::GetInstance()->Paused() == false)
				Engine::GetInstance()->SetPause(true);
			else
				Engine::GetInstance()->SetPause(false);
		}
		Name_WinScene::CanPause = false;
	}
	else
	{
		Name_WinScene::CanPause = true;
	}

#ifdef _DEBUG

	if (inputHandler->keyPressed(SDL_SCANCODE_GRAVE) && Name_WinScene::TbdCanToggleFullBright == true)
	{
		cheatFullbright();

		if (!Name_WinScene::isGravePressedForCheat)
		{
			Name_WinScene::isGravePressedForCheat = true;
		}
		else
		{
			Name_WinScene::isGravePressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_GRAVE))
	{
		Name_WinScene::TbdCanToggleFullBright = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_Q) && Name_WinScene::TbdCanToggleOnlyLights == true)
	{
		cheatOnlyLights();

		if (!Name_WinScene::isQPressedForCheat)
		{
			Name_WinScene::isQPressedForCheat = true;
		}
		else
		{
			Name_WinScene::isQPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_Q))
	{
		Name_WinScene::TbdCanToggleOnlyLights = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_N) && Name_WinScene::TbdCanToggleNormalDisplay == true)
	{
		cheatNormalMap();

		if (!Name_WinScene::isNPressedForCheat)
		{
			Name_WinScene::isNPressedForCheat = true;
		}
		else
		{
			Name_WinScene::isNPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_N))
	{
		Name_WinScene::TbdCanToggleNormalDisplay = true;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_C) && Name_WinScene::TbdCanRenderWallHitboxes == 1)
	{
		cheatWallHitboxes();

		if (!Name_WinScene::isCPressedForCheat)
		{
			Name_WinScene::isCPressedForCheat = true;
		}
		else
		{
			Name_WinScene::isCPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_C))
	{
		Name_WinScene::TbdCanRenderWallHitboxes = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_F) && Name_WinScene::canToggleScanLines == 1)
	{
		cheatScanlines();

		if (!Name_WinScene::isFPressedForCheat)
		{
			Name_WinScene::isFPressedForCheat = true;
		}
		else
		{
			Name_WinScene::isFPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_F))
	{
		Name_WinScene::canToggleScanLines = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_H) && Name_WinScene::canRenderRawFog == 1)
	{
		cheatOnlyFog();

		if (!Name_WinScene::isHPressedForCheat)
		{
			Name_WinScene::isHPressedForCheat = true;
		}
		else
		{
			Name_WinScene::isHPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_H))
	{
		Name_WinScene::canRenderRawFog = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_G) && Name_WinScene::canToggleFog == 1)
	{
		cheatFog();

		if (!Name_WinScene::isGPressedForCheat)
		{
			Name_WinScene::isGPressedForCheat = true;
		}
		else
		{
			Name_WinScene::isGPressedForCheat = false;
		}
	}
	if (!inputHandler->keyPressed(SDL_SCANCODE_G))
	{
		Name_WinScene::canToggleFog = 1;
	}
#endif
}

static float timer = 0.0f;
void WinScene::Update(float dt)
{
	if (CheckGameScenes() || CheckRestart())
		return;
	timer += dt;

	//eventally have player handle these lights
	//TbdPixelRenderer->lightSource[1].position = TbdPixelRenderer->animatedObjects[0][0]->position + Vector2{ 3,3 };
	//TbdPixelRenderer->lightSource[0].position = TbdPixelRenderer->animatedObjects[0][0]->position + Vector2{ 3,3 };

	if (timer >= 2.0f)
	{
		Vector2 moveUp = WinSceneRenderer->GetCameraPosition();
		if (moveUp.y > 148.0f) //-48 from start
		{
			Vector2 ScreenHalfSize = 0.5f * Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
			moveUp.y -= (((ScreenHalfSize.y + 4.0f) / 64.0f) * dt) * .2f;
			WinSceneRenderer->SetCameraPosition({ moveUp.x, moveUp.y });
		}
		else
		{
			Transform* trans = (*EntityContainer::GetInstance())[0]->GetComponent<Transform>();
			if (trans->GetTranslation()->y < 154)
			{
				Vector2 translation = (*EntityContainer::GetInstance())[0]->GetImage()->position;
				translation.y += (40 * dt);

				//(*EntityContainer::GetInstance())[0]->GetComponent<Transform>()->SetTranslation(translation);
				(*EntityContainer::GetInstance())[0]->GetImage()->position.y = translation.y;
			}
		}
	}

	Inputs* inputHandler = Inputs::GetInstance();
	AudioManager.Update();
	inputHandler->handleInput();

	Name_WinScene::TbdPlayerMovement(dt);
	handleCheatCodes();

	ImGuiInterg();
	WinSceneRenderer->Update(dt);
	entityManagerTBD->pInput = Inputs::GetInstance();
}

void WinScene::Render()
{
	return;
}

Engine::EngineCode WinScene::Exit()
{
	EntityContainer::GetInstance()->FreeAll();
	LevelBuilder::SetWinState(false);
	LevelBuilder::setDoor(false);
	Inputs::GetInstance()->InputKeyClear();
	WinSceneRenderer->CleanRenderer();
	return Engine::NothingBad;
}

Engine::EngineCode WinScene::Unload()
{
	WinSceneinstance->~Scene();
	WinSceneRenderer = nullptr;
	return Engine::NothingBad;
}

Scene* WinSceneGetInstance(void)
{
	static Scene* TbdSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!WinSceneinstance)
	{
		WinSceneinstance = new WinScene();
	}
	return WinSceneinstance;
}

#ifndef ImGUI_Functions

void WinScene::ImGuiInterg()
{
#ifdef _DEBUG
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (Name_WinScene::show_custom_window)
	{
		ImGuiWindow();
	}

	ImGui::Render();
#endif
}

void WinScene::ImGuiWindow()
{
#ifdef _DEBUG
	if (Name_WinScene::show_custom_window)
	{
		ImGui::Begin("TBDTestScene");
		ImGui::BeginMainMenuBar();

		if (ImGui::MenuItem("Reset Scene", NULL, false, true))
			SceneSystem::GetInstance()->RestartScene();
		if (ImGui::MenuItem("Test Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
		if (ImGui::MenuItem("TbdTest Scene", NULL, false, true))
			SceneSystem::GetInstance()->SetScene(WinSceneGetInstance());

		ImGui::EndMainMenuBar();

		ImGui::Text("have fun,");
		ImGui::Text("welcome to the test window!");
		ImGui::Separator();

		if (ImGui::Button("Toggle Metrics/Debug Bar"))
		{
			Name_WinScene::show_metrics_debug_bar = !Name_WinScene::show_metrics_debug_bar;
		}

		if (Name_WinScene::show_metrics_debug_bar)
		{
			ImGui::Text("Metrics/Debugger:");
			ImGui::Separator();
			ImGui::Text("Frame Time: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

			int mouseX = Inputs::GetInstance()->getMouseX();
			int mouseY = Inputs::GetInstance()->getMouseY();
			float screenScale = WinSceneRenderer->screenScale;
			float cameraPosX = WinSceneRenderer->GetCameraPosition().x;
			float cameraPosY = WinSceneRenderer->GetCameraPosition().y;
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

			ImGui::SliderFloat("Brightness", &WinSceneRenderer->minBrightness, 0.0f, 255.0f);
			ImGui::SliderFloat("Saturation", &WinSceneRenderer->saturationPercent, 0.0f, 1.0f);

			ImGui::Separator();
		}

		ImGui::Separator();

		ImGui::Text("Cheats:");

		if (ImGui::Button("FullBright Cheat:"))
		{
			Name_WinScene::isGravePressedForCheat = !Name_WinScene::isGravePressedForCheat;

			cheatFullbright();
		}
		ImGui::SameLine();
		ImGui::Text(Name_WinScene::isGravePressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Only Lights Cheat:"))
		{
			Name_WinScene::isQPressedForCheat = !Name_WinScene::isQPressedForCheat;

			cheatOnlyLights();
		}
		ImGui::SameLine();
		ImGui::Text(Name_WinScene::isQPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("NormalMap Cheat:"))
		{
			Name_WinScene::isNPressedForCheat = !Name_WinScene::isNPressedForCheat;

			cheatNormalMap();
		}
		ImGui::SameLine();
		ImGui::Text(Name_WinScene::isNPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Wall Hitboxes Cheat:"))
		{
			Name_WinScene::isCPressedForCheat = !Name_WinScene::isCPressedForCheat;

			cheatWallHitboxes();
		}
		ImGui::SameLine();
		ImGui::Text(Name_WinScene::isCPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("ScanLines Cheat:"))
		{
			Name_WinScene::isFPressedForCheat = !Name_WinScene::isFPressedForCheat;

			cheatScanlines();
		}

		ImGui::SameLine();
		ImGui::Text(Name_WinScene::isFPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Blur Cheat:"))
		{
			Name_WinScene::isEPressedForCheat = !Name_WinScene::isEPressedForCheat;

			cheatBlur();
		}

		ImGui::SameLine();
		ImGui::Text(Name_WinScene::isEPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Justa Fog Cheat:"))
		{
			Name_WinScene::isHPressedForCheat = !Name_WinScene::isHPressedForCheat;

			cheatOnlyFog();
		}

		ImGui::SameLine();
		ImGui::Text(Name_WinScene::isHPressedForCheat ? "Active" : "Inactive");

		if (ImGui::Button("Render Fog Cheat:"))
		{
			Name_WinScene::isGPressedForCheat = !Name_WinScene::isGPressedForCheat;

			cheatFog();
		}

		ImGui::SameLine();
		ImGui::Text(Name_WinScene::isGPressedForCheat ? "Active" : "Inactive");

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

		Particle** particles = WinSceneRenderer->particleManager->GetParticles();

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