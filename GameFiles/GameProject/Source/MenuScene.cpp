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

Logging& MenuLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* MenuRender;
Renderer* MenuPixelRender = Renderer::GetInstance();

SDL_Window* MenuWindow;

SDL_GLContext MenuGlContext;

Scene* MenuSceneinstance = NULL;


MenuScene::MenuScene() : Scene("Menutest")
{

}

Engine::EngineCode MenuScene::Load()
{
    //AudioManager.LoadMusicFromJSON("./Data/music.json");
    //AudioManager.LoadSFXFromJSON("./Data/SFX.json");

    return Engine::NothingBad;
}

Engine::EngineCode MenuScene::Init()
{
    Inputs::GetInstance()->SetWindow(MenuWindow);

    //exporttests
    //FileIO::GetInstance()->ExportTileMap("export_tests");

    Light tempLight;
    Light tempLight2;

    // Create SDL Window
    MenuWindow = PlatformSystem::GetInstance()->GetWindowHandle();
    MenuPixelRender->window = MenuWindow;

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
    int numTestDust = 120;
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

    FontSystem fontSystem;

    fontSystem.init("Font/MouldyCheeseRegular-WyMWG.ttf", 10);

    Engine::GetInstance()->SetPause(false);
    return Engine::NothingBad;
}

#ifndef Render_Toggle_Functions
int MenuCanPlaceLight = 0;
bool MenuCanToggleFullBright = true;
bool MenuCanToggleNormalDisplay = true;
bool MenuCanToggleOnlyLights = true;
bool MenuCanPause = true;
bool MenucanToggleScanLines = true;
bool MenuCanRenderWallHitboxes = true;
bool MenucanRenderRawFog = true;
bool MenucanToggleFog = true;

static bool MenutabKeyPreviouslyPressed = false;
static bool Menushow_demo_window = false;
static bool Menushow_tool_metrics = false;
static bool Menushow_custom_window = false;
static bool Menushow_metrics_debug_bar = false;

static bool MenuisGravePressedForCheat = false;
static bool MenuisQPressedForCheat = false;
static bool MenuisNPressedForCheat = false;
static bool MenuisCPressedForCheat = false;
static bool MenuisFPressedForCheat = false;
static bool MenuisEPressedForCheat = false;
static bool MenuisHPressedForCheat = false;
static bool MenuisGPressedForCheat = false;


void MenuScene::cheatFullbright()
{
    if (MenuPixelRender->isFullbright == false)
        MenuPixelRender->isFullbright = true;
    else
        MenuPixelRender->isFullbright = false;
    MenuCanToggleFullBright = false;
}

void MenuScene::cheatBlur()
{
    if (MenuPixelRender->doBlur == false)
        MenuPixelRender->doBlur = true;
    else
        MenuPixelRender->doBlur = false;
    MenuCanPlaceLight = 0;
}

void MenuScene::cheatOnlyLights()
{
    if (MenuPixelRender->renderOnlyLights == false)
        MenuPixelRender->renderOnlyLights = true;
    else
        MenuPixelRender->renderOnlyLights = false;
    MenuCanToggleOnlyLights = false;
}

void MenuScene::cheatNormalMap()
{
    if (MenuPixelRender->renderNormalMap == false)
        MenuPixelRender->renderNormalMap = true;
    else
        MenuPixelRender->renderNormalMap = false;
    MenuCanToggleNormalDisplay = false;
}

void MenuScene::cheatWallHitboxes()
{
    if (MenuPixelRender->renderWallHitboxes == false)
        MenuPixelRender->renderWallHitboxes = true;
    else
        MenuPixelRender->renderWallHitboxes = false;
    MenuCanRenderWallHitboxes = 0;
}

void MenuScene::cheatScanlines()
{
    if (MenuPixelRender->doScanLines == false)
        MenuPixelRender->doScanLines = true;
    else
        MenuPixelRender->doScanLines = false;
    MenucanToggleScanLines = 0;
}

void MenuScene::cheatOnlyFog()
{
    if (MenuPixelRender->drawRawFog == false)
        MenuPixelRender->drawRawFog = true;
    else
        MenuPixelRender->drawRawFog = false;
    MenucanRenderRawFog = 0;
}

void MenuScene::cheatFog()
{
    if (MenuPixelRender->doFog == false)
        MenuPixelRender->doFog = true;
    else
        MenuPixelRender->doFog = false;
    MenucanToggleFog = 0;
}

void MenuScene::handleCheatCodes()
{
    Inputs* inputHandler = Inputs::GetInstance();

    if (inputHandler->keyPressed(SDL_SCANCODE_E) && MenuCanPlaceLight == 1)
    {
        cheatBlur();

        if (!MenuisEPressedForCheat)
        {
            MenuisEPressedForCheat = true;
        }
        else
        {
            MenuisEPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_E))
    {
        MenuCanPlaceLight = 1;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_GRAVE) && MenuCanToggleFullBright == true)
    {
        cheatFullbright();

        if (!MenuisGravePressedForCheat)
        {
            MenuisGravePressedForCheat = true;
        }
        else
        {
            MenuisGravePressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_GRAVE))
    {
        MenuCanToggleFullBright = true;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_Q) && MenuCanToggleOnlyLights == true)
    {
        cheatOnlyLights();

        if (!MenuisQPressedForCheat)
        {
            MenuisQPressedForCheat = true;
        }
        else
        {
            MenuisQPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_Q))
    {
        MenuCanToggleOnlyLights = true;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_N) && MenuCanToggleNormalDisplay == true)
    {
        cheatNormalMap();

        if (!MenuisNPressedForCheat)
        {
            MenuisNPressedForCheat = true;
        }
        else
        {
            MenuisNPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_N))
    {
        MenuCanToggleNormalDisplay = true;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_LSHIFT))
    {
        if (MenuCanPause == true)
        {
            AudioManager.PlaySFX("creak");
            if (Engine::GetInstance()->Paused() == false)
                Engine::GetInstance()->SetPause(true);
            else
                Engine::GetInstance()->SetPause(false);
        }
        MenuCanPause = false;
    }
    else
    {
        MenuCanPause = true;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_C) && MenuCanRenderWallHitboxes == 1)
    {
        cheatWallHitboxes();

        if (!MenuisCPressedForCheat)
        {
            MenuisCPressedForCheat = true;
        }
        else
        {
            MenuisCPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_C))
    {
        MenuCanRenderWallHitboxes = 1;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_F) && MenucanToggleScanLines == 1)
    {
        cheatScanlines();

        if (!MenuisFPressedForCheat)
        {
            MenuisFPressedForCheat = true;
        }
        else
        {
            MenuisFPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_F))
    {
        MenucanToggleScanLines = 1;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_H) && MenucanRenderRawFog == 1)
    {
        cheatOnlyFog();

        if (!MenuisHPressedForCheat)
        {
            MenuisHPressedForCheat = true;
        }
        else
        {
            MenuisHPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_H))
    {
        MenucanRenderRawFog = 1;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_G) && MenucanToggleFog == 1)
    {
        cheatFog();

        if (!MenuisGPressedForCheat)
        {
            MenuisGPressedForCheat = true;
        }
        else
        {
            MenuisGPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_G))
    {
        MenucanToggleFog = 1;
    }
}

#endif

void MenuPlayerMovement(float dt)
{
    Inputs* inputHandler = Inputs::GetInstance();

    if (inputHandler->keyPressed(SDL_SCANCODE_TAB))
    {
        if (!MenutabKeyPreviouslyPressed)
        {
            Menushow_demo_window = !Menushow_demo_window;
            Menushow_tool_metrics = !Menushow_tool_metrics;
            Menushow_custom_window = !Menushow_custom_window;

        }
        MenutabKeyPreviouslyPressed = true;
    }
    else
    {
        MenutabKeyPreviouslyPressed = false;
    }

    if (Engine::GetInstance()->Paused() == false)
    {
        /*
        int x, y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);

        //This is the Scenes Player light tracking code or at least part of it
        // assuming Cursour Player and Light Player are the naming schemes.
        Vector2 CursourP = { (float)x, (float)y };
        CursourP *= 1.0f / MenuPixelRender->screenScale;
        CursourP += MenuPixelRender->GetCameraPosition();

        Vector2 LightP = MenuPixelRender->lightSource[0].position;
        //Direction? Difference?
        Vector2 D = LightP - CursourP;
        float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;
        MenuPixelRender->lightSource[0].angle = Angle;

        ImageBuffer* playerEntity = MenuPixelRender->animatedObjects[0][0];
        Vector2 ScreenHalfSize = 0.5f * Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
        Vector2 BitmapHalfDim = 0.5f * playerEntity->size;
        MenuPixelRender->SetCameraPosition(playerEntity->position - ScreenHalfSize + BitmapHalfDim);
        */
        //move the laser
        /*
        if (MenuLaser)
        {
            Vector2 Delta = CursourP - MenuLaser->Position;
            float Angle = atan2f(Delta.y, Delta.x);
            MenuLaser->Direction = Vector2(cosf(Angle), sinf(Angle));
        }
        */
    }

}

void MenuScene::Update(float dt)
{
    if (CheckGameScenes() || CheckRestart())
        return;

    //eventally have player handle these lights
    MenuPixelRender->lightSource[1].position = MenuPixelRender->animatedObjects[0][0]->position + Vector2{ 3,3 };
    MenuPixelRender->lightSource[0].position = MenuPixelRender->animatedObjects[0][0]->position + Vector2{ 3,3 };

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
            MenuPixelRender->menuBuffer = new ImageBuffer{ SCREEN_SIZE_X,SCREEN_SIZE_Y };
            ImageBuffer* temp = new ImageBuffer{ "./Assets/PPM/TemporaryWinScreen.ppm" };
            MenuPixelRender->menuBuffer->AddSprite(temp, Vector2{ -20,-5 });
            Engine::GetInstance()->SetPause(true);
        }
        if (inputHandler->keyPressed(SDL_SCANCODE_RETURN))
        {
            SceneSystem::RestartScene();
        }

    }
    MenuPlayerMovement(dt);
    handleCheatCodes();

    ImGuiInterg();
    MenuPixelRender->Update(dt);
}

void MenuScene::Render()
{
    return;
}

Engine::EngineCode MenuScene::Exit()
{
    EntityContainer::GetInstance()->FreeAll();
    LevelBuilder::SetWinState(false);
    LevelBuilder::setDoor(false);
    Inputs::GetInstance()->InputKeyClear();
    MenuPixelRender->CleanRenderer();
    winState = false;
    return Engine::NothingBad;
}

Engine::EngineCode MenuScene::Unload()
{
    winState = false;
    delete MenuSceneinstance;
    MenuSceneinstance = nullptr;
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

    if (Menushow_custom_window)
    {
        ImGuiWindow();
    }

    ImGui::Render();
#endif
}

void MenuScene::ImGuiWindow()
{
#ifdef _DEBUG
    if (Menushow_custom_window)
    {
        ImGui::Begin("custom window");
        ImGui::Text("hey how you doin ;)");
        ImGui::Text("welcome to the MenuScene debug window");

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
            MenuisGravePressedForCheat = !MenuisGravePressedForCheat;

            cheatFullbright();
        }
        ImGui::SameLine();
        ImGui::Text(MenuisGravePressedForCheat ? "Active" : "Inactive");

        if (ImGui::Button("Render Only Lights Cheat:"))
        {
            MenuisQPressedForCheat = !MenuisQPressedForCheat;

            cheatOnlyLights();
        }
        ImGui::SameLine();
        ImGui::Text(MenuisQPressedForCheat ? "Active" : "Inactive");

        if (ImGui::Button("NormalMap Cheat:"))
        {
            MenuisNPressedForCheat = !MenuisNPressedForCheat;

            cheatNormalMap();
        }
        ImGui::SameLine();
        ImGui::Text(MenuisNPressedForCheat ? "Active" : "Inactive");

        if (ImGui::Button("Wall Hitboxes Cheat:"))
        {
            MenuisCPressedForCheat = !MenuisCPressedForCheat;

            cheatWallHitboxes();
        }
        ImGui::SameLine();
        ImGui::Text(MenuisCPressedForCheat ? "Active" : "Inactive");

        if (ImGui::Button("ScanLines Cheat:"))
        {
            MenuisFPressedForCheat = !MenuisFPressedForCheat;

            cheatScanlines();
        }

        ImGui::SameLine();
        ImGui::Text(MenuisFPressedForCheat ? "Active" : "Inactive");

        if (ImGui::Button("Blur Cheat:"))
        {
            MenuisEPressedForCheat = !MenuisEPressedForCheat;

            cheatBlur();
        }

        ImGui::SameLine();
        ImGui::Text(MenuisEPressedForCheat ? "Active" : "Inactive");

        if (ImGui::Button("Render Justa Fog Cheat:"))
        {
            MenuisHPressedForCheat = !MenuisHPressedForCheat;

            cheatOnlyFog();
        }

        ImGui::SameLine();
        ImGui::Text(MenuisHPressedForCheat ? "Active" : "Inactive");

        if (ImGui::Button("Render Fog Cheat:"))
        {
            MenuisGPressedForCheat = !MenuisGPressedForCheat;

            cheatFog();
        }

        ImGui::SameLine();
        ImGui::Text(MenuisGPressedForCheat ? "Active" : "Inactive");

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
            Menushow_metrics_debug_bar = !Menushow_metrics_debug_bar;
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

        if (Menushow_metrics_debug_bar)
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