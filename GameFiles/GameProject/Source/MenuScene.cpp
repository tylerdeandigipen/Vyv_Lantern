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

