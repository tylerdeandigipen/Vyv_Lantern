//------------------------------------------------------------------------------
//
// File Name:	WinScene.cpp
// Author(s):	Michael Howard
// Purpose:		Change this to whatever you need
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "WinScene.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <algorithm>

#include "TbdTestScene.h"
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

#include "TestScene.h"



SDL_Renderer* WinRenderer;
Renderer* WinPixelRenderer = Renderer::GetInstance();

SDL_Window* WinWindow;

SDL_GLContext WinGlContext;

Scene* WinSceneinstance = NULL;

static bool tabKeyPreviouslyPressed = false;
static bool show_demo_window = false;
static bool show_tool_metrics = false;
static bool show_custom_window = false;
static bool show_metrics_debug_bar = false;

static bool isGravePressedForCheat = false;
static bool isQPressedForCheat = false;
static bool isNPressedForCheat = false;
static bool isCPressedForCheat = false;


WinScene::WinScene() : Scene("Win")
{

}

WinScene::~WinScene()
{

}

Engine::EngineCode WinScene::Init()
{
    Inputs::GetInstance()->SetWindow(WinWindow);
	WinWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	WinPixelRenderer->window = WinWindow;
    FileIO::GetInstance()->ReadTileSet("./Data/TileMapSprites.json", WinPixelRenderer);
	LevelBuilder::GetInstance()->LoadTileMap("./MichaelTempWinScene.json");


    //WinPixelRenderer->GenerateVornoiPoints();

	return Engine::NothingBad;
}

Engine::EngineCode WinScene::Load()
{
	return Engine::NothingBad;
}

void WinScene::Update(float dt)
{
    if (CheckGameScenes() || CheckRestart())
        return;

    Vector2 ScreenHalfSize = 0.5f * Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
    WinPixelRenderer->SetCameraPosition(ScreenHalfSize);

    Inputs* inputHandler = Inputs::GetInstance();
    inputHandler->handleInput();
    ImGuiInterg();
	WinPixelRenderer->Update(dt);

}

void WinScene::Render()
{
	return;
}

Engine::EngineCode WinScene::Unload()
{
    WinSceneinstance->~Scene();
    WinSceneinstance = nullptr;
	return Engine::NothingBad;
}

Engine::EngineCode WinScene::Exit()
{
    LevelBuilder::GetInstance()->FreeLevel();
    Inputs::GetInstance()->InputKeyClear();
    WinPixelRenderer->CleanRenderer();
	return Engine::NothingBad;
}

Scene* WinSceneGetInstance(void)
{
	static Scene* TbdSceneinstance = nullptr; // Make it static to ensure a single instance
	if (!WinSceneinstance) {
		WinSceneinstance = new WinScene();
	}
	return WinSceneinstance;
}

void WinScene::ImGuiInterg()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (show_custom_window)
    {
        ImGuiWindow();
    }

    ImGui::Render();
}

void WinScene::ImGuiWindow()
{
    if (show_custom_window)
    {
        ImGui::Begin("custom window");
        ImGui::Text("hey how you doin ;)");
        ImGui::Text("welcome to the tbdtestscene debug window");

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

        ImGui::Separator();
        ImGui::Text("Cheat Status:");
        ImGui::Text("` Cheat: %s", isGravePressedForCheat ? "Active" : "Inactive");
        ImGui::Text("Q Cheat: %s", isQPressedForCheat ? "Active" : "Inactive");
        ImGui::Text("N Cheat: %s", isNPressedForCheat ? "Active" : "Inactive");
        ImGui::Text("C Cheat: %s", isCPressedForCheat ? "Active" : "Inactive");

        ImGui::Text("Particle System:");
        ImGui::Separator();

        Particle** particles = WinPixelRenderer->particleManager->GetParticles();

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

        if (ImGui::Button("Toggle Metrics/Debug Bar"))
        {
            show_metrics_debug_bar = !show_metrics_debug_bar;
        }

        if (ImGui::Button("Reset Scene"))
        {
            SceneSystem::GetInstance()->RestartScene();
        }

        if (ImGui::Button("Switch Scene"))
        {
            SceneSystem::GetInstance()->SetScene(TbdTestSceneGetInstance());
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
}