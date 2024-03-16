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

Logging& MenuLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* MenuRender;
Renderer* MenuPixelRender = Renderer::GetInstance();

SDL_Window* MenuWindow;

Scene* MenuSceneinstance = NULL;

Entity* Beginbutton;
Entity* ExitButton;

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

    cheatScanlines();

    //exporttests
    json ignore{ {"isAnimated", false}};
    Beginbutton = new Entity("Object", "./Assets/PPM/Begin.ppm", ignore);
    ExitButton = new Entity("Object", "./Assets/PPM/quit.ppm", ignore);

    Renderer::GetInstance()->isFullbright = true;

    Beginbutton->CreateImage("./Assets/PPM/Begin.ppm");
    ExitButton->CreateImage("./Assets/PPM/quit.ppm");

    Transform* BPOS = new Transform;
    BPOS->SetTranslation(gfxVector2{70,25});
    Beginbutton->Add(BPOS);
    Beginbutton->AddToRenderer(MenuPixelRender, "");

    Transform* BBPOS = new Transform;
    BBPOS->SetTranslation(gfxVector2{ 70,65 });
    ExitButton->Add(BBPOS);
    ExitButton->AddToRenderer(MenuPixelRender, "");
    

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


void MenuScene::cheatScanlines()
{
    if (MenuPixelRender->doScanLines == false)
        MenuPixelRender->doScanLines = true;
    else
        MenuPixelRender->doScanLines = false;
}


bool MenuScene::IsMouseOverBeginingButton()
{
    int mouseX = Inputs::GetInstance()->getMouseX();
    int mouseY = Inputs::GetInstance()->getMouseY();


    Vector2 position = {720,255};

    int width = 100;
    int high = 50;


    int LeftOfButton = position.x - width;
    int TopOfButton = position.y - high;
    int RightOfButton = position.x + width;
    int ButtomOfButton = position.y + high;

    return (mouseX >=  LeftOfButton && mouseX <= RightOfButton &&
        mouseY >= TopOfButton && mouseY <= ButtomOfButton);

}


void MenuScene::HandleBegin()
{
    if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
    {
        SceneSystem* sceneSystem = SceneSystem::GetInstance();
        sceneSystem->SetScene(TbdTestSceneGetInstance());

    }
}

bool MenuScene::IsMouseOverExitButton()
{
    int mouseX = Inputs::GetInstance()->getMouseX();
    int mouseY = Inputs::GetInstance()->getMouseY();


    Vector2 position = { 720,455 };

    int width = 100;
    int high = 50;


    int LeftOfButton = position.x - width;
    int TopOfButton = position.y - high;
    int RightOfButton = position.x + width;
    int ButtomOfButton = position.y + high;

    return (mouseX >= LeftOfButton && mouseX <= RightOfButton &&
        mouseY >= TopOfButton && mouseY <= ButtomOfButton);
}




void MenuScene::HandleExit()
{
    if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
    {
        Engine::GetInstance()->SetCloseRequest(true);
    }
}

void MenuScene::Update(float dt)
{
    if (CheckGameScenes() || CheckRestart())
        return;

    Inputs* inputHandler = Inputs::GetInstance();
    AudioManager.Update();
    inputHandler->handleInput();
    bool check = winState;

    MenuPixelRender->RenderMenu();
    Beginbutton->Update(dt);
    ExitButton->Update(dt);

    if (IsMouseOverBeginingButton())
    {
        HandleBegin();
        //Renderer::GetInstance()->isFullbright = false;

    }
    if (IsMouseOverExitButton())
    {
        HandleExit();
    }

    MenuPixelRender->Update(dt);
}

void MenuScene::Render()
{
    return;
}

Engine::EngineCode MenuScene::Exit()
{
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

