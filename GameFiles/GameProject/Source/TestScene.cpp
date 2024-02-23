//------------------------------------------------------------------------------
//
// File Name:	TestScene.cpp
// Author(s):	Tyler Dean, Michael Howard, TayLee Young,
//              Thomas Stephenson, Louis Wang
// Purpose:     Main laser for the game.
//
// Copyright  ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Engine.h"
#include "Entity.h"
#include "EntityContainer.h"
#include "ImageBuffer.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "Inputs.h"
#include "LevelBuilder.h"
#include "LevelCreatorScene.h"
#include "Light.h"
#include "Logging.h"
#include "PlatformSystem.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "TbdTestScene.h"
#include "TestScene.h"
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>

Logging& logger = Logging::GetInstance("debugLog.log");

ImageBuffer* testSprite;
ImageBuffer* testSprite1;
ImageBuffer* topwall, * bottomwall, * leftwall, * rightwall;

Entity* testEntity;
Entity* jsonEntity;
SDL_Renderer* renderer;

SDL_Window* window;
Renderer* pixelRenderer = Renderer::GetInstance();

bool canMove = true; // Initialize to allow movement

SDL_GLContext glContext; // OpenGL context for SDL2

Scene* TestSceneinstance = NULL; // ITS A GLOBAL VARIABLE CALM DOWN!! SHOW ME ANOTHER WAY AND ITS GONE

Color white(255, 255, 255, 255);
Color black(0, 0, 0, 255);
Color grey(150, 150, 150, 255);
Color blue(50, 100, 255, 255);

//float soundCooldown = 0.0f;

int ObjCount = 0;
const float pushForce = 1.0f;

static bool tabKeyPreviouslyPressed = false;
static bool show_demo_window = false;

static bool TStabKeyPreviouslyPressed = false;
static bool TSshow_demo_window = false;
static bool TSshow_tool_metrics = false;
static bool TSshow_custom_window = true;
static bool TSshow_metrics_debug_bar = false;

/* NOTICE !!!!!!!!!! Feel free to "turn off" debug messages as you please. You can see them in the debugLog.txt in the game files, or in the output tab when debugging. Literally
   you can call for the logger anywhere as long as you get an instance first. Use it in ur functions or something - taylee */

TestScene::TestScene() : Scene("test")
{
}

Engine::EngineCode TestScene::Load()
{
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Init()
{
	{
		ControlledEmitter = LaserSystem::GetInstance()->CreateEmitter();

		// @NOTE: Setup lasers
		laser_emitter* TestLaser = LaserSystem::GetInstance()->GetEmitter(ControlledEmitter);
		TestLaser->Position = Vector2(40.0f, 60.0f);
		TestLaser->Direction = Vector2::Normalize(Vector2(0.0f, 1.0f));

		laser_emitter* TestLaser1 = NewEmitter();
		TestLaser1->Position = Vector2(200.0f, 100.0f);
		TestLaser1->Direction = Vector2::Normalize(Vector2(-1.0f, 0.0f));

		reflector* Reflector = NewReflector();
		Reflector->Radius = 10.0f;
		Reflector->Position = Vector2(100.0f, 100.0f);
		Reflector->Direction = Vector2::Normalize(Vector2(1.0f, 1.0f));

		reflector* Reflector1 = NewReflector();
		Reflector1->Radius = 20.0f;
		Reflector1->Position = Vector2(100.0f, 30.0f);
		Reflector1->Direction = Vector2::Normalize(Vector2(0.0f, 1.0f));

		//reflector *Reflector2 = NewReflector();
		//Reflector2->Radius = 100.0f;
		//Reflector2->Position = Vector2(200.0f, 50.0f);
		//Reflector2->Direction = Vector2::Normalize(Vector2(1.0f, 1.0f));
	}

	/*BGM*/

	//AudioManager.PlayMusic("bgm.ogg");

	logger.LogToAll("Timestamped message: %s", "Starting TestScene!!");

	Inputs::GetInstance()->SetWindow(window);

	Light tempLight;
	Light tempLight2;
	Light tempLight3;

	Color transparent(0, 0, 0, 0);

	// Create SDL Window
	window = PlatformSystem::GetInstance()->GetWindowHandle();
	pixelRenderer->window = window;

	logger.LogLine("Debug info: Lights, camera, action! (testScene init)");
	return Engine::NothingBad;
}

int canPlaceMoreLight = 0;
float moveSpeed = 20;
void tempPlayerMovementLol(float dt)
{
	Inputs* inputHandler = Inputs::GetInstance();
	if (canMove)
	{
		if (inputHandler->keyPressed(SDL_SCANCODE_W))
		{
			pixelRenderer->objects[2]->position.y -= moveSpeed * dt;

			logger.LogLine("Debug info: Vyv Up pressed.");

			//pixelRenderer.AddLight(pixelRenderer.staticLightSource[0]);
			//AudioManager.PlaySFX("footsteps.ogg");
		}
		if (inputHandler->keyPressed(SDL_SCANCODE_S))
		{
			pixelRenderer->objects[2]->position.y += moveSpeed * dt;

			logger.LogLine("Debug info: Vyv Down pressed.");

			//AudioManager.PlaySFX("footsteps.ogg");
		}
		if (inputHandler->keyPressed(SDL_SCANCODE_D))
		{
			pixelRenderer->objects[2]->position.x += moveSpeed * dt;

			logger.LogLine("Debug info: Vyv Right pressed.");

			//AudioManager.PlaySFX("footsteps.ogg");
		}
		if (inputHandler->keyPressed(SDL_SCANCODE_A))
		{
			pixelRenderer->objects[2]->position.x -= moveSpeed * dt;

			logger.LogLine("Debug info: Vyv Left pressed.");

			//AudioManager.PlaySFX("footsteps.ogg");
		}
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_E) && canPlaceMoreLight == 1)
	{
		pixelRenderer->AddLight(pixelRenderer->lightSource[0]);
		canPlaceMoreLight = 0;
	}

	if (!inputHandler->keyPressed(SDL_SCANCODE_E))
	{
		canPlaceMoreLight = 1;
	}

	if (inputHandler->keyPressed(SDL_SCANCODE_TAB))
	{
		if (!TStabKeyPreviouslyPressed)
		{
			TSshow_demo_window = !TSshow_demo_window;
			TSshow_tool_metrics = !TSshow_tool_metrics;
			TSshow_custom_window = !TSshow_custom_window;
		}
		TStabKeyPreviouslyPressed = true;
	}
	else
	{
		TStabKeyPreviouslyPressed = false;
	}

	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);

	Vector2 CursourP = { (float)x, (float)y };
	CursourP *= 1.0f / pixelRenderer->screenScale;
	CursourP += pixelRenderer->GetCameraPosition();

	Vector2 LightP = pixelRenderer->lightSource[0].position;
	Vector2 D = LightP - CursourP;
	float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;

	// pixelRenderer->lightSource[0].angle = Angle;

	ImageBuffer* playerEntity = pixelRenderer->objects[1];
	Vector2 ScreenHalfSize = 0.5f * Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	Vector2 BitmapHalfDim = 0.5f * playerEntity->size;
	pixelRenderer->SetCameraPosition(playerEntity->position - ScreenHalfSize + BitmapHalfDim);
}

void TestScene::Update(float dt)
{
	if (CheckGameScenes() || CheckRestart())
		return;

	Inputs* inputHandler = Inputs::GetInstance();

	//LevelBuilder::GetInstance()->LevelUpdate(dt);
	AudioManager.Update();
	inputHandler->handleInput();

	if (inputHandler->keyPressed(SDLK_ESCAPE) == true)
	{
		Engine::GetInstance()->SetCloseRequest(true);
	}

	{
		int CursourX = 0;
		int CursourY = 0;
		uint32_t Buttons = SDL_GetMouseState(&CursourX, &CursourY);

		Vector2 CursourP = Vector2((float)CursourX, (float)CursourY);
		CursourP *= 1.0f / pixelRenderer->screenScale;
		CursourP += pixelRenderer->GetCameraPosition();

		laser_emitter* Emitter = LaserSystem::GetInstance()->GetEmitter(ControlledEmitter);

		if (Emitter)
		{
			Vector2 Delta = CursourP - Emitter->Position;
			float Angle = atan2f(Delta.y, Delta.x);
			Emitter->Direction = Vector2(cosf(Angle), sinf(Angle));
		}
	}

	pixelRenderer->Update(dt);
}

void TestScene::Render()
{
#ifdef _DEBUG
	ImGuiInterg();
#endif
#if 1
	{
		// NOTE(thomas): Demonstration of beam path iteration
		laser_emitter* Emitter = LaserSystem::GetInstance()->GetEmitter(ControlledEmitter);
		Vector2 LastNodeP = Emitter->Position;
		LaserSystem::beam_path_iterator Iterator = LaserSystem::GetInstance()->IterateEmitterPath(ControlledEmitter);
		while (Iterator.IsValid())
		{
			beam_path_node* Node = Iterator.CurrentNode;
			Renderer::GetInstance()->DrawLine(Node->Position, Node->Position + Vector2(0, -4), Color(0x00, 0xff, 0xff, 0xff));

			LastNodeP = Node->Position;
			Iterator.Advance();
		}
	}
#endif
	return;
}

Engine::EngineCode TestScene::Exit()
{
	// Remember to clean up
	Inputs::GetInstance()->InputKeyClear();
	pixelRenderer->CleanRenderer();

	logger.LogLine("Debug info: Bye-Bye! (testScene exit)");
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Unload()
{
	delete TestSceneinstance;
	TestSceneinstance = nullptr;

	LevelBuilder::GetInstance()->FreeLevel();

	logger.LogLine("Debug info: entities destroyed :( (testScene unloaded)");
	return Engine::NothingBad;
}

Scene* TestSceneGetInstance(void)
{
	static Scene* TestSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!TestSceneinstance) {
		TestSceneinstance = new TestScene();
	}
	return TestSceneinstance;
}

/**********************************************************************/
void TestScene::ImGuiInterg()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (TSshow_custom_window)
	{
#ifdef _DEBUG
		ImGuiWindow();
#endif
	}

	ImGui::Render();
}

void TestScene::ImGuiWindow()
{
	if (TSshow_custom_window)
	{
		ImGui::Begin("custom window");
		ImGui::Text("hey how you doin ;)");

		int numEntities = EntityContainer::GetInstance()->CountEntities();
		ImGui::Text("Number of Entities: %d", numEntities);

		ImGui::Text("Mouse Position: (%d, %d)", Inputs::GetInstance()->getMouseX(), Inputs::GetInstance()->getMouseY());

		ImGui::Text("Keys Pressed:");
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
		{
			if (Inputs::GetInstance()->keyPressed(i))
			{
				const char* keyName = SDL_GetKeyName(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(i)));
				ImGui::BulletText("%s is being pressed", keyName);
			}
		}

		ImGui::Text("Particle System:");
		ImGui::Separator();

		Particle** particles = pixelRenderer->particleManager->GetParticles();

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

		if (ImGui::TreeNode("Tilesets in use:"))
		{
			ImGui::BulletText("TileMapSprites.json");
			ImGui::BulletText("TileMapNormals.json");
			ImGui::TreePop();
		}

		if (ImGui::Button("Reset Scene"))
		{
			SceneSystem::GetInstance()->RestartScene();
		}

		if (ImGui::Button("TbdTest Scene"))
		{
			SceneSystem::GetInstance()->SetScene(TbdTestSceneGetInstance());
		}

		if (ImGui::Button("Level Creator Scene"))
		{
			SceneSystem::GetInstance()->SetScene(LevelCreatorSceneGetInstance());
		}

		if (ImGui::Button("Toggle Metrics/Debug Bar"))
		{
			TSshow_metrics_debug_bar = !TSshow_metrics_debug_bar;
		}

		if (TSshow_metrics_debug_bar)
		{
			ImGui::Text("Metrics/Debugger:");
			ImGui::Separator();
			ImGui::Text("Frame Time: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

			ImGui::Separator();
		}

		ImGui::End();
	}
}