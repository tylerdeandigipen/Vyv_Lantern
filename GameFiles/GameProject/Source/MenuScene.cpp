//------------------------------------------------------------------------------
//
// File Name:	MenuScene.cpp
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
#include "MenuScene.h"
#include "LevelCreatorScene.h"
#include "Scene.h"
#include "Engine.h"

#include "FileIO.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "Inputs.h"
#include "Logging.h"
#include "Time.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "PlatformSystem.h"
#include "Scene.h"
#include "Engine.h"
#include "TestScene.h"
#include "TBDTestScene.h"
#include "LevelCreatorScene.h"
#include "SplashScene.h"
#include "Inputs.h"
#include "MenuScene.h"
#include "Entity.h"
#include "ImageBuffer.h"
#include "LevelBuilder.h"
#include "FontSystem.h"
#include "Component.h"
#include "TestScene.h"
#include "Transform.h"
#include "Sprite.h"

#include "DebugNew.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Logging& MenuLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* MenuRender;
Renderer* MenuPixelRender = Renderer::GetInstance();

SDL_Window* MenuWindow;

Scene* MenuSceneinstance = NULL;

static Entity* Beginbutton;
static Entity* Creditbutton;
static Entity* optionbutton;
static Entity* ExitButton;
static Entity* Title;

//Entity* PlayerIcon;

ImageBuffer* ani;

MenuScene::MenuScene() : Scene("Menutest"), isCreditsOpen(false), isConfirmQuitOpen(false), isOptionsOpen(false)
{
	offset = -50.0f;
}

Engine::EngineCode MenuScene::Load()
{
	AudioManager.LoadMusicFromJSON("./Data/music.json");
	AudioManager.LoadSFXFromJSON("./Data/SFX.json");

	return Engine::NothingBad;
}

Engine::EngineCode MenuScene::Init()
{
	Inputs::GetInstance()->SetWindow(MenuWindow);

	// Create SDL Window
	MenuWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	MenuPixelRender->window = MenuWindow;

	//cheatScanlines();

	//exporttests
	json ignore{ {"isAnimated", false} };

	//json ingnore{ {"isAnimated", true,}, {"frameSize", {8,8}} };

	Beginbutton = new Entity("Object", "./Assets/PPM/Begin.ppm", ignore);
	ExitButton = new Entity("Object", "./Assets/PPM/quit.ppm", ignore);
	Creditbutton = new Entity("Object", "./Assets/PPM/credit.ppm", ignore);
	optionbutton = new Entity("Object", "./Assets/PPM/option.ppm", ignore);

	Title = new Entity("Object", "./Assets/PPM/title.ppm", ignore);

	//PlayerIcon = new Entity("Object", "./Assets/PPM/Player_Sprites.ppm", ingnore);

	Renderer::GetInstance()->isFullbright = true;

	Beginbutton->CreateImage("./Assets/PPM/Begin.ppm");
	ExitButton->CreateImage("./Assets/PPM/quit.ppm");
	Creditbutton->CreateImage("./Assets/PPM/credit.ppm");
	optionbutton->CreateImage("./Assets/PPM/option.ppm");
	Title->CreateImage("./Assets/PPM/title.ppm");

	ani = new ImageBuffer();
	ani = Renderer::GetInstance()->CreateAnimatedObject("./Assets/PPM/menu_animation.ppm", Vector2{ 80,80 });

	ani->position = gfxVector2{ 122,45 };
	ani->isFlipped = false;

	//MenuPixelRender->objectLayer = new ImageBuffer;

	//ani->AddSprite(ani, Vector2{30,30});

	Transform* BPOS = new Transform;
	BPOS->SetTranslation(gfxVector2{ offset + 92,38 });
	Beginbutton->Add(BPOS);
	Beginbutton->AddToRenderer(MenuPixelRender, "");

	Transform* crepos = new Transform;
	crepos->SetTranslation(gfxVector2{ offset + 92,60 });
	Creditbutton->Add(crepos);
	Creditbutton->AddToRenderer(MenuPixelRender, "");

	Transform* optpos = new Transform;
	optpos->SetTranslation(gfxVector2{ offset + 92,82 });
	optionbutton->Add(optpos);
	optionbutton->AddToRenderer(MenuPixelRender, "");

	Transform* BBPOS = new Transform;
	BBPOS->SetTranslation(gfxVector2{ offset + 92,104 });
	ExitButton->Add(BBPOS);
	ExitButton->AddToRenderer(MenuPixelRender, "");

	Transform* TitlePos = new Transform;
	TitlePos->SetTranslation(gfxVector2{ offset + 94,5 });
	Title->Add(TitlePos);
	Title->AddToRenderer(MenuPixelRender, "");

	MenuPixelRender->TurnoffFace();

	//Transform* MCP = new Transform;
	//MCP->SetTranslation(gfxVector2{ 42,65 });
	//PlayerIcon->Add(MCP);
	//PlayerIcon->AddToRenderer(MenuPixelRender, "");

	//initialize level data
	//EntityContainer::GetInstance()->ReadEntities();
	//LevelBuilder::GetInstance()->LoadTileMap("./Data/Scenes/TutorialFinal2.json");

	//ControlledEmitter = LaserSystem::GetInstance()->CreateEmitter();
	/*
	laser_emitter* MenuLaser1 = NewEmitter();
	MenuLaser1->Position = Vector2(200.0f, 50.0f + 12.0f); //add offset to accomodate for the position of the mirror (opengl position mixed with sdl2 position)
	MenuLaser1->Direction = Vector2::Normalize(Vector2(-1.0f, 0.0f));
	*/

	Color tempColor{ 141,141,141,255 };
	int numTestDust = 140;
	Vector2 tempRandNum;
	for (int i = 0; i < numTestDust; i++)
	{
		tempRandNum = Vector2{ (float)(rand() % SCREEN_SIZE_X - 10), (float)(rand() % SCREEN_SIZE_Y - 10) };
		tempRandNum += Vector2{ 10,10 };
		Particle* testParticle = new Particle(tempRandNum, Vector2{ -0.8f,-0.25f }, Vector2{ 17.0f, 15.0f }, tempColor, Particle_Dust);
		MenuPixelRender->particleManager->AddParticle(testParticle);
	}

	//AudioManager.PlayMusic("drips");

	//AudioManager.PlayMusic("forest");

	return Engine::NothingBad;
}

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

void MenuScene::cheatScanlines()
{
	if (MenuPixelRender->doScanLines == false)
		MenuPixelRender->doScanLines = true;
	else
		MenuPixelRender->doScanLines = false;
}

void MenuScene::Update(float dt)
{
	if (CheckGameScenes() || CheckRestart())
		return;

	Inputs* inputHandler = Inputs::GetInstance();
	AudioManager.Update();
	inputHandler->handleInput();
	bool check = winState;

	MenuPixelRender->TurnoffFace();

	MenuPixelRender->RenderMenu();
	Beginbutton->Update(dt);
	ExitButton->Update(dt);
	Creditbutton->Update(dt);
	optionbutton->Update(dt);
	Title->Update(dt);

	//PlayerIcon->Update(dt);

	if (IsMouseOverBeginingButton())
	{
		HandleBegin();
	}
	if (IsMouseOverCreditButton())
	{
		//HandleExit();
		HandleCredit();
	}
	if (IsMouseOverOptionButton())
	{
		//HandleExit();
		HandleOption();
	}
	if (IsMouseOverExitButton())
	{
		HandleExit();
	}

	if (isCreditsOpen && !isConfirmQuitOpen && !isOptionsOpen)
	{
		openCredits();
	}

	if (isConfirmQuitOpen && !isCreditsOpen && !isOptionsOpen)
	{
		openConfirmQuitMenu();
	}

	if (isOptionsOpen && !isCreditsOpen && !isConfirmQuitOpen)
	{
		openOptions();
	}

	if (IsMouseOverExitButtonYes() && isConfirmQuitOpen == true)
	{
		closeConfirmQuitMenu();
	}

	if (IsMouseOverExitButtonNo() && isConfirmQuitOpen == true)
	{
		closeConfirmQuitMenu();
	}

	if (IsMouseOverCloseCredit() && isCreditsOpen)
	{
		closeCredits();
	}

	MenuPixelRender->Update(dt);
}

void MenuScene::openConfirmQuitMenu()
{
	Renderer::GetInstance()->LoadMenuPage(Renderer::GetInstance()->confirmQuitMainindex);
}

void MenuScene::closeConfirmQuitMenu()
{
	if (IsMouseOverExitButtonYes())
	{
		if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			isConfirmQuitOpen = false;

			Engine::GetInstance()->SetCloseRequest(true);
		}
	}

	if (IsMouseOverExitButtonNo())
	{
		if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			isConfirmQuitOpen = false;

			Renderer::GetInstance()->LoadMenuPage(-1);
		}
	}
}

void MenuScene::openCredits()
{
	Renderer::GetInstance()->LoadMenuPage(Renderer::GetInstance()->creditsMainMenuIndex);
}

void MenuScene::closeCredits()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);
		isCreditsOpen = false;

		Renderer::GetInstance()->LoadMenuPage(-1);
		isConfirmQuitOpen = false;
	}
}

void MenuScene::openOptions()
{
	Renderer::GetInstance()->LoadMenuPage(Renderer::GetInstance()->mainMenuOptionsIndex);
}

void MenuScene::closeOptions()
{
}

void MenuScene::HandleBegin()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);
		SceneSystem* sceneSystem = SceneSystem::GetInstance();
		Renderer::GetInstance()->isFullbright = false;
		sceneSystem->SetScene(TbdTestSceneGetInstance());
	}
}

void MenuScene::HandleOption()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (!isCreditsOpen && !isConfirmQuitOpen && !isOptionsOpen)
		{
			//do something
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			isOptionsOpen = true;
		}
	}
}

void MenuScene::HandleCredit()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (!isCreditsOpen && !isConfirmQuitOpen)
		{
			//do something
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			isCreditsOpen = true;
		}
	}
}

void MenuScene::HandleExit()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (!isConfirmQuitOpen && !isCreditsOpen)
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			isConfirmQuitOpen = true;
		}
	}
}

bool MenuScene::IsMouseOverBeginingButton()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	Vector2 position = { 400,300 };

	int width = (250 / 2);
	int high = (128 / 2);

	int LeftOfButton = position.x - width;
	int TopOfButton = position.y - high;
	int RightOfButton = position.x + width;
	int ButtomOfButton = position.y + high;

	return (mouseX >= LeftOfButton && mouseX <= RightOfButton &&
		mouseY >= TopOfButton && mouseY <= ButtomOfButton);
}

bool MenuScene::IsMouseOverOptionButton()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	Vector2 position = { 400,550 };

	int width = (250 / 2);
	int high = (128 / 2);

	int LeftOfButton = position.x - width;
	int TopOfButton = position.y - high;
	int RightOfButton = position.x + width;
	int ButtomOfButton = position.y + high;

	return (mouseX >= LeftOfButton && mouseX <= RightOfButton &&
		mouseY >= TopOfButton && mouseY <= ButtomOfButton);
}

bool MenuScene::IsMouseOverCreditButton()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	Vector2 position = { 400,400 };

	int width = (250 / 2);
	int high = (128 / 2);

	int LeftOfButton = position.x - width;
	int TopOfButton = position.y - high;
	int RightOfButton = position.x + width;
	int ButtomOfButton = position.y + high;

	return (mouseX >= LeftOfButton && mouseX <= RightOfButton &&
		mouseY >= TopOfButton && mouseY <= ButtomOfButton);
}

bool MenuScene::IsMouseOverCloseCredit()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int exitButtonTopLeftX = 366;
	int exitButtonTopLeftY = 691;
	int exitButtonBottomRightX = 550;
	int exitButtonBottomRightY = 798;

	return (mouseX >= exitButtonTopLeftX && mouseX <= exitButtonBottomRightX &&
		mouseY >= exitButtonTopLeftY && mouseY <= exitButtonBottomRightY);
}

bool MenuScene::IsMouseOverExitButton()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	Vector2 position = { 400,700 };

	int width = (250 / 2);
	int high = (128 / 2);

	int LeftOfButton = position.x - width;
	int TopOfButton = position.y - high;
	int RightOfButton = position.x + width;
	int ButtomOfButton = position.y + high;

	return (mouseX >= LeftOfButton && mouseX <= RightOfButton &&
		mouseY >= TopOfButton && mouseY <= ButtomOfButton);
}

bool MenuScene::IsMouseOverExitButtonYes()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int exitButtonTopLeftX = 527;
	int exitButtonTopLeftY = 468;
	int exitButtonBottomRightX = 684;
	int exitButtonBottomRightY = 560;

	return (mouseX >= exitButtonTopLeftX && mouseX <= exitButtonBottomRightX &&
		mouseY >= exitButtonTopLeftY && mouseY <= exitButtonBottomRightY);
}

bool MenuScene::IsMouseOverExitButtonNo()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int exitButtonTopLeftX = 758;
	int exitButtonTopLeftY = 470;
	int exitButtonBottomRightX = 910;
	int exitButtonBottomRightY = 556;

	return (mouseX >= exitButtonTopLeftX && mouseX <= exitButtonBottomRightX &&
		mouseY >= exitButtonTopLeftY && mouseY <= exitButtonBottomRightY);
}

void MenuScene::Render()
{
	return;
}

Engine::EngineCode MenuScene::Exit()
{
	delete Beginbutton->GetComponent<Transform>();
	delete ExitButton->GetComponent<Transform>();
	delete Creditbutton->GetComponent<Transform>();
	delete optionbutton->GetComponent<Transform>();

	delete Beginbutton;
	delete ExitButton;
	delete Creditbutton;
	delete optionbutton;

	Beginbutton = NULL;
	ExitButton = NULL;
	Creditbutton = NULL;
	optionbutton = NULL;

	delete ani;
	ani = NULL;
	Inputs::GetInstance()->InputKeyClear();
	MenuPixelRender->CleanRenderer();
	return Engine::NothingBad;
}

Engine::EngineCode MenuScene::Unload()
{
	//delete MenuSceneinstance;
	//MenuSceneinstance = nullptr;

	AudioManager.UnLoadSFX("buttonFeedback");
	return Engine::NothingBad;
}

Scene* MenuSceneGetInstance(void)
{
	static Scene* TbdSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!MenuSceneinstance) {
		MenuSceneinstance = new MenuScene();
	}
	return MenuSceneinstance;
}

#ifndef ImGUI_Functions

void MenuScene::ImGuiInterg()
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

void MenuScene::ImGuiWindow()
{
#ifdef _DEBUG
	if (show_custom_window)
	{
		ImGui::Begin("custom window");
		ImGui::Text("hey how you doin ;)");
		ImGui::Text("welcome to the tbdtestscene debug window");

		ImGui::Separator();

		int mouseX = Inputs::GetInstance()->getMouseX();
		int mouseY = Inputs::GetInstance()->getMouseY();
		float screenScale = MenuPixelRender->screenScale;
		float cameraPosX = MenuPixelRender->GetCameraPosition().x;
		float cameraPosY = MenuPixelRender->GetCameraPosition().y;
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

		ImGui::Separator();
		ImGui::Text("Cheat Status:");

		if (ImGui::Button("ScanLines Cheat:"))
		{
			isFPressedForCheat = !isFPressedForCheat;

			cheatScanlines();
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

		ImGui::Separator();

		ImGui::Text("Particle System:");

		Particle** particles = MenuPixelRender->particleManager->GetParticles();

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

		if (ImGui::TreeNode("Tilesets in use:"))
		{
			ImGui::BulletText("TileMapSprites.json");
			ImGui::BulletText("TileMapNormals.json");
			ImGui::TreePop();
		}

		ImGui::Separator();
		ImGui::Text("Scene Tools:");

		if (ImGui::Button("Toggle Metrics/Debug Bar"))
		{
			show_metrics_debug_bar = !show_metrics_debug_bar;
		}

		if (ImGui::Button("Reset Scene"))
		{
			SceneSystem::GetInstance()->RestartScene();
		}

		if (ImGui::Button("Test Scene"))
		{
			SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
		}

		if (ImGui::Button("Level Creator Scene"))
		{
			SceneSystem::GetInstance()->SetScene(LevelCreatorSceneGetInstance());
		}

		if (show_metrics_debug_bar)
		{
			ImGui::Text("Metrics/Debugger:");
			ImGui::Separator();
			ImGui::Text("Frame Time: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

			ImGui::Separator();
		}

		ImGui::End();
	}
#endif
}
#endif