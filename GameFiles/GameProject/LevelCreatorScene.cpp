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
#include "TbdTestScene.h"

Logging& LevelCreatorLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* LevelCreatorRenderer;
Renderer* LevelCreatorPixelRenderer;

SDL_Window* LevelCreatorWindow;

SDL_GLContext LevelCreatorGlContext;

Scene* LevelCreatorSceneinstance = NULL; 

static bool showCreatorToolsWindow = true;

LevelCreatorScene::LevelCreatorScene() : Scene("LevelCreatortest")
{

}

Engine::EngineCode LevelCreatorScene::Load()
{
	return Engine::NothingBad;
}

int currentTool = 0;
int currentTile;
Vector2 moveVector;
Vector2 oldMousePos;
Vector2 previousTile;
Engine::EngineCode LevelCreatorScene::Init()
{
    LevelCreatorPixelRenderer = Renderer::GetInstance();
    Inputs::GetInstance()->SetWindow(LevelCreatorWindow);

    LevelCreatorWindow = PlatformSystem::GetInstance()->GetWindowHandle();
    LevelCreatorPixelRenderer->window = LevelCreatorWindow;

    //initialize level data
   // EntityContainer::GetInstance()->ReadEntities("./Data/GameObjects/ObjectListLevelBuilder.json");
    LevelBuilder::GetInstance()->LoadLevel("./Data/Tbd_Testing_Level_Master/Tbd_Testing_Level.json");
    LevelCreatorPixelRenderer->window = LevelCreatorWindow;

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
int expansionRange = 3;

void LevelCreatorScene::ToolEyedroper(Inputs* inputHandler, Vector2 CursourP)
{
    if (inputHandler->mouseButtonDown(SDL_BUTTON_LEFT))
    {
        Vector2 tilePos = (CursourP + LevelCreatorPixelRenderer->GetCameraPosition());
        tilePos /= 8;
        if ((int)tilePos.x < (int)LevelCreatorPixelRenderer->tileMapSize.x && (int)tilePos.x > 0 && (int)tilePos.y < (int)LevelCreatorPixelRenderer->tileMapSize.y && (int)tilePos.y > 0)
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
        pos1 = tilePos;
        wasDown = true;
    }
    if (inputHandler->mouseButtonUp(SDL_BUTTON_LEFT) && wasDown == true)
    {
        pos2 = tilePos;
        if ((int)((int)pos2.y - (int)pos1.y) != 0 && (int)((int)pos2.x - (int)pos1.x) != 0)
        {
            Vector2 desiredTile = pos1;
            for (int i = 0; i < 2; i++)
            {
                if ((int)desiredTile.x + 1 < (int)LevelCreatorPixelRenderer->tileMapSize.x && (int)desiredTile.x - 1 > 0 && (int)desiredTile.y + 1 < (int)LevelCreatorPixelRenderer->tileMapSize.y && (int)desiredTile.y - 1 > 0)
                {

                }
                else
                {
                    if (currentTile == 0 || currentTile == 4)
                    {
                        break;
                    }
                    if ((int)desiredTile.x < 0)
                    {
                        LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ -1,0 }, expansionRange + abs(desiredTile.x));
                        pos1.x += expansionRange + (int)abs(desiredTile.x);
                        pos2.x += expansionRange + (int)abs(desiredTile.x);

                        moveVector.x += expansionRange + (int)abs(desiredTile.x) * TILE_SIZE;
                    }
                    if ((int)desiredTile.x > LevelCreatorPixelRenderer->tileMapSize.x)
                    {
                        LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 1,0 }, expansionRange + (desiredTile.x - LevelCreatorPixelRenderer->tileMapSize.x));
                    }
                    if ((int)desiredTile.y < 0)
                    {
                        LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 0,-1 }, expansionRange);
                        pos1.y += expansionRange + abs(desiredTile.y);
                        pos2.y += expansionRange + abs(desiredTile.y);
                        moveVector.y += expansionRange * TILE_SIZE;
                    }
                    if ((int)desiredTile.y > LevelCreatorPixelRenderer->tileMapSize.y)
                    {
                        LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 0,1 }, expansionRange + (desiredTile.y - LevelCreatorPixelRenderer->tileMapSize.y));
                    }

                    if ((int)desiredTile.x != (int)previousTile.x || (int)desiredTile.y != (int)previousTile.y)
                    {
                        LevelCreatorPixelRenderer->SetCameraPosition(moveVector);
                    }
                }

                desiredTile = pos2;
            }

            int moveByOneX = ((int)pos2.x - (int)pos1.x) / abs((int)pos2.x - (int)pos1.x);
            int moveByOneY = ((int)pos2.y - (int)pos1.y) / abs((int)pos2.y - (int)pos1.y);
            for (int x = (int)pos1.x; x != (int)pos2.x + moveByOneX; x += moveByOneX)
            {
                for (int y = (int)pos1.y; y != (int)pos2.y + moveByOneY; y += moveByOneY)
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
        }
        wasDown = false;
    }
}

void LevelCreatorScene::ToolBrush(Inputs* inputHandler, Vector2 CursourP)
{
    if (inputHandler->mouseButtonPressed(SDL_BUTTON_LEFT))
    {
        Vector2 tilePos = (CursourP + LevelCreatorPixelRenderer->GetCameraPosition());
        tilePos /= 8;
        if ((int)tilePos.x < (int)LevelCreatorPixelRenderer->tileMapSize.x && (int)tilePos.x >= 0 && (int)tilePos.y < (int)LevelCreatorPixelRenderer->tileMapSize.y && (int)tilePos.y >= 0)
        {
            if (currentTile == 0 || currentTile == 4)
            {
                LevelCreatorPixelRenderer->TileMapEraseTile(tilePos);
                return;
            }
            LevelCreatorPixelRenderer->TileMapSetTile(tilePos, currentTile);
        }
        else
        {
            if (currentTile == 0 || currentTile == 4)
            {
                return;
            }
            if ((int)tilePos.x - 1 < 0)
            {
                LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ -1,0 }, expansionRange);
                tilePos.x = expansionRange;
                moveVector.x += expansionRange * TILE_SIZE;
            }
            if ((int)tilePos.x + 1 > LevelCreatorPixelRenderer->tileMapSize.x)
            {
                LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 1,0 }, expansionRange + (tilePos.x - LevelCreatorPixelRenderer->tileMapSize.x));
            }
            if ((int)tilePos.y - 1 < 0)
            {
                LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 0,-1 }, expansionRange);
                tilePos.y = expansionRange;
                moveVector.y += expansionRange * TILE_SIZE;
            }
            if ((int)tilePos.y + 1 > LevelCreatorPixelRenderer->tileMapSize.y)
            {
                LevelCreatorPixelRenderer->ExpandTileMapInDirection(Vector2{ 0,1 }, expansionRange + (tilePos.y - LevelCreatorPixelRenderer->tileMapSize.y));
            }
            LevelCreatorPixelRenderer->TileMapSetTile(tilePos, currentTile);
        }
        if ((int)tilePos.x != (int)previousTile.x || (int)tilePos.y != (int)previousTile.y)
        {
            previousTile = tilePos;

            LevelCreatorPixelRenderer->SetCameraPosition(moveVector);
        }
    }
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

void LevelCreatorScene::ToolHandler()
{
    Inputs* inputHandler = Inputs::GetInstance();

    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);
    Vector2 CursourP = { (float)x, (float)y };
    CursourP *= 1.0f / LevelCreatorPixelRenderer->screenScale;

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
    else if (inputHandler->mouseButtonPressed(SDL_BUTTON_LEFT))
    {
        currentTool = 0;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_B))
    {
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

void LevelCreatorScene::Update(float dt)
{
    if (CheckGameScenes() || CheckRestart())
        return;

    Inputs* inputHandler = Inputs::GetInstance();
    inputHandler->handleInput();
    ImGuiInterg();

    ToolHandler();

    LevelCreatorPixelRenderer->isFullbright = true;

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

/* --------------------------------------------------------------------------------------------------------------------------------- */

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

        ImGui::Begin("Design Tools");
        ImGui::Text("have fun designers,");
        ImGui::Text("welcome to the creator window!");

        ImGui::Separator();

        if (ImGui::TreeNode("Export:"))
        {
            ImGui::InputText(".json", myTextBuffer, sizeof(myTextBuffer));
            if (ImGui::Button("Submit")) {
                FileIO::GetInstance()->ExportTileMap(myTextBuffer);
            }
        }

        ImGui::Separator();

        if (ImGui::TreeNode("Tools:"))
        {
            if (ImGui::Button("Center"))
            {
                currentTool = 3;
            }

            if (ImGui::Button("EyeDropper"))
            {
                currentTool = 2;
            }

            if (ImGui::Button("Erase"))
            {
                currentTool = 0;
                currentTile = 0;
            }

            if (ImGui::Button("SquareFill"))
            {
                currentTool = 1;
            }

            if (ImGui::TreeNode("Tile Selector:"))
            {
                if (ImGui::Button("Wall Tile"))
                {
                    currentTile = 1;
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        ImGui::Separator();

        if (ImGui::Button("Reset Scene"))
        {
            SceneSystem::GetInstance()->RestartScene();
        }

        if (ImGui::Button("Test Scene"))
        {
            SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
        }

        if (ImGui::Button("TbdTest Scene"))
        {
            SceneSystem::GetInstance()->SetScene(TbdTestSceneGetInstance());
        }

        ImGui::End();
    }
}