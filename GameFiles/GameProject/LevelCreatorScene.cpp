//------------------------------------------------------------------------------
//
// File Name:	LevelCreatorScene.cpp
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

Logging& LevelCreatorLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* LevelCreatorRenderer;
Renderer* LevelCreatorPixelRenderer = Renderer::GetInstance();

SDL_Window* LevelCreatorWindow;

SDL_GLContext LevelCreatorGlContext;

Scene* LevelCreatorSceneinstance = NULL; 

LevelCreatorScene::LevelCreatorScene() : Scene("LevelCreatortest")
{

}

Engine::EngineCode LevelCreatorScene::Load()
{
	return Engine::NothingBad;
}

Engine::EngineCode LevelCreatorScene::Init()
{
    LevelCreatorWindow = PlatformSystem::GetInstance()->GetWindowHandle();
    LevelCreatorPixelRenderer->window = LevelCreatorWindow;

    Inputs::GetInstance()->SetWindow(LevelCreatorWindow);

    //initialize level data
    EntityContainer::GetInstance()->ReadEntities("./Data/GameObjects/ObjectList.json");
    LevelBuilder::GetInstance()->LoadLevel("./Data/Tbd_Testing_Level_Master/Tbd_Testing_Level.json");
    
    return Engine::NothingBad;
}

#ifndef Render_Toggle_Functions

bool LevelCreatorCanToggleFullBright = true;


static bool tabKeyPreviouslyPressed = false;
static bool show_demo_window = false;
static bool show_tool_metrics = false;
static bool show_custom_window = false;
static bool show_metrics_debug_bar = false;

static bool isGravePressedForCheat = false;




void LevelCreatorScene::cheatFullbright()
{
    if (LevelCreatorPixelRenderer->isFullbright == false)
        LevelCreatorPixelRenderer->isFullbright = true;
    else
        LevelCreatorPixelRenderer->isFullbright = false;
    LevelCreatorCanToggleFullBright = false;
}

void LevelCreatorPlayerMovement(float dt)
{
    Inputs* inputHandler = Inputs::GetInstance();

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

    if (Engine::GetInstance()->Paused() == false)
    {
        int x, y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);

        Vector2 CursourP = { (float)x, (float)y };
        CursourP *= 1.0f / LevelCreatorPixelRenderer->screenScale;
        CursourP += LevelCreatorPixelRenderer->GetCameraPosition();
    }
}
#endif

void LevelCreatorScene::Update(float dt)
{
    if (CheckGameScenes() || CheckRestart())
        return;

    Inputs* inputHandler = Inputs::GetInstance();
    inputHandler->handleInput();

   // LevelCreatorPlayerMovement(dt);
   // cheatFullbright();

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
    if (!LevelCreatorSceneinstance) {
        LevelCreatorSceneinstance = new LevelCreatorScene();
    }
    return LevelCreatorSceneinstance;
}

#ifndef ImGUI_Functions

void LevelCreatorScene::ImGuiInterg()
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

void LevelCreatorScene::ImGuiWindow()
{
#ifdef _DEBUG
    if (show_custom_window)
    {
        ImGui::Begin("custom window");
        ImGui::Text("hey how you doin ;)");
        ImGui::Text("welcome to the LevelCreatorScene debug window");

        ImGui::Separator();

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

        if (ImGui::Button("FullBright Cheat:"))
        {
            isGravePressedForCheat = !isGravePressedForCheat;

            cheatFullbright();
        }
        ImGui::SameLine();
        ImGui::Text(isGravePressedForCheat ? "Active" : "Inactive");

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

        if (ImGui::Button("Switch Scene"))
        {
            SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
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