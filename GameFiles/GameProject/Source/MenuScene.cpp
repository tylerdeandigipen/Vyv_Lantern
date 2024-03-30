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



Logging& MenuLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* MenuRender;
Renderer* MenuPixelRender = Renderer::GetInstance();

SDL_Window* MenuWindow;

Scene* MenuSceneinstance = NULL;

Entity* Beginbutton;
Entity* Creditbutton;
Entity* optionbutton;
Entity* ExitButton;

//Entity* PlayerIcon;

ImageBuffer* ani;

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
    //json ingnore{ {"isAnimated", true,}, {"frameSize", {8,8}} };


    Beginbutton = new Entity("Object", "./Assets/PPM/Begin.ppm", ignore);
    ExitButton = new Entity("Object", "./Assets/PPM/quit.ppm", ignore);
    Creditbutton = new Entity("Object", "./Assets/PPM/credit.ppm", ignore);
    optionbutton = new Entity("Object", "./Assets/PPM/option.ppm", ignore);

    //PlayerIcon = new Entity("Object", "./Assets/PPM/Player_Sprites.ppm", ingnore);

    Renderer::GetInstance()->isFullbright = true;

    Beginbutton->CreateImage("./Assets/PPM/Begin.ppm");
    ExitButton->CreateImage("./Assets/PPM/quit.ppm");
    Creditbutton->CreateImage("./Assets/PPM/credit.ppm");
    optionbutton->CreateImage("./Assets/PPM/option.ppm");


    ani = new ImageBuffer();
    ani = Renderer::GetInstance()->CreateAnimatedObject("./Assets/PPM/Player_Sprites.ppm", Vector2{8,8});

    ani->position = gfxVector2{ 42,65 };
    ani->isFlipped = true;

    //MenuPixelRender->objectLayer = new ImageBuffer;

    //ani->AddSprite(ani, Vector2{30,30});


    Transform* BPOS = new Transform;
    BPOS->SetTranslation(gfxVector2{92,25});
    Beginbutton->Add(BPOS);
    Beginbutton->AddToRenderer(MenuPixelRender, "");

    
    Transform* crepos = new Transform;
    crepos->SetTranslation(gfxVector2{ 92,50 });
    Creditbutton->Add(crepos);
    Creditbutton->AddToRenderer(MenuPixelRender, "");

    Transform* optpos = new Transform;
    optpos->SetTranslation(gfxVector2{ 92,75 });
    optionbutton->Add(optpos);
    optionbutton->AddToRenderer(MenuPixelRender, "");

    Transform* BBPOS = new Transform;
    BBPOS->SetTranslation(gfxVector2{ 92,100 });
    ExitButton->Add(BBPOS);
    ExitButton->AddToRenderer(MenuPixelRender, "");


    //Transform* MCP = new Transform;
    //MCP->SetTranslation(gfxVector2{ 42,65 });
    //PlayerIcon->Add(MCP);
    //PlayerIcon->AddToRenderer(MenuPixelRender, "");

    // Create SDL Window
    MenuWindow = PlatformSystem::GetInstance()->GetWindowHandle();
    MenuPixelRender->window = MenuWindow;



    FontSystem fontSystem;

    fontSystem.init("Font/MouldyCheeseRegular-WyMWG.ttf", 10);

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


bool MenuScene::IsMouseOverBeginingButton()
{
    int mouseX = Inputs::GetInstance()->getMouseX();
    int mouseY = Inputs::GetInstance()->getMouseY();


    Vector2 position = {700,215};

    int width = (250/2);
    int high = (128/2);


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


    Vector2 position = { 700,690 };

    int width = (250 / 2);
    int high = (128 / 2);


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


bool MenuScene::IsMouseOverCreditButton()
{
    int mouseX = Inputs::GetInstance()->getMouseX();
    int mouseY = Inputs::GetInstance()->getMouseY();


    Vector2 position = { 700,375 };

    int width = (250 / 2);
    int high = (128 / 2);


    int LeftOfButton = position.x - width;
    int TopOfButton = position.y - high;
    int RightOfButton = position.x + width;
    int ButtomOfButton = position.y + high;

    return (mouseX >= LeftOfButton && mouseX <= RightOfButton &&
        mouseY >= TopOfButton && mouseY <= ButtomOfButton);
}

void MenuScene::HandleCredit()
{
    if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
    {
        //do something
    }
}


bool MenuScene::IsMouseOverOptionButton()
{
    int mouseX = Inputs::GetInstance()->getMouseX();
    int mouseY = Inputs::GetInstance()->getMouseY();


    Vector2 position = { 700,520 };

    int width = (250 / 2);
    int high = (128 / 2);


    int LeftOfButton = position.x - width;
    int TopOfButton = position.y - high;
    int RightOfButton = position.x + width;
    int ButtomOfButton = position.y + high;

    return (mouseX >= LeftOfButton && mouseX <= RightOfButton &&
        mouseY >= TopOfButton && mouseY <= ButtomOfButton);
}

void MenuScene::HandleOption()
{
    if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
    {
        //do something
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
    Creditbutton->Update(dt);
    optionbutton->Update(dt);

    //PlayerIcon->Update(dt);

    if (IsMouseOverBeginingButton())
    {
        HandleBegin();
        //Renderer::GetInstance()->isFullbright = false;

    }
    if (IsMouseOverCreditButton())
    {
        HandleCredit();
    }
    if (IsMouseOverOptionButton())
    {
        HandleOption();
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