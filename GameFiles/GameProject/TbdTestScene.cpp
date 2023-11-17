//------------------------------------------------------------------------------
//
// File Name:	TbdTestScene.cpp
// Author(s):	Tyler Dean
// Purpose:     My test scene dont edit plz
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

Logging& TbdLogger = Logging::GetInstance("debugLog.log");

SDL_Renderer* TbdRenderer;
Renderer* TbdPixelRenderer = Renderer::GetInstance();

SDL_Window* TbdWindow;

SDL_GLContext TbdGlContext;

Scene* TbdTestSceneinstance = NULL; 

static bool tabKeyPreviouslyPressed = false;
static bool show_demo_window = false;
static bool show_tool_metrics = false;
static bool show_custom_window = false;
static bool show_metrics_debug_bar = false;

bool isGravePressedForCheat = false;
bool isQPressedForCheat = false;
bool isNPressedForCheat = false;
bool isCPressedForCheat = false;

TbdTestScene::TbdTestScene() : Scene("tbdtest")
{

}

Engine::EngineCode TbdTestScene::Load()
{
    AudioManager.LoadMusicFromJSON("./Data/music.json");
    AudioManager.LoadSFXFromJSON("./Data/SFX.json");

	return Engine::NothingBad;
}

Engine::EngineCode TbdTestScene::Init()
{
    TbdLogger.LogToAll("Timestamped message: %s", "Starting TbdTestScene!!");

    Inputs::GetInstance()->SetWindow(TbdWindow);

    Light tempLight;
    Light tempLight2;

    // Create SDL Window
    TbdWindow = PlatformSystem::GetInstance()->GetWindowHandle();
    TbdPixelRenderer->window = TbdWindow;

    TbdGlContext = SDL_GL_CreateContext(TbdWindow);
    SDL_GL_SetSwapInterval(0);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    FileIO::GetInstance()->ReadTileSet("./Data/TileMapSprites.json", TbdPixelRenderer);
    FileIO::GetInstance()->ReadTileSet("./Data/TileMapNormals.json", TbdPixelRenderer, 1);
    FileIO::GetInstance()->ReadTileSet("./Data/TileMapShadows.json", TbdPixelRenderer, 2);
    LevelBuilder::GetInstance()->LoadLevel(TbdPixelRenderer, "./TiledMichaelTest.json");

    FileIO::GetInstance()->ReadLight("./Data/TestLight.json", tempLight);

    InitImGui();

    tempLight2.position.x = 200;
    tempLight2.position.y = 90;

    tempLight2.color = { 216, 247, 255, 255 };

    tempLight2.intensity = 3.0f;
    tempLight2.radius = 20;
    tempLight2.radialFalloff = 3;
    tempLight2.radialWeight = 1;
    tempLight2.angularWeight = 0;
    tempLight2.volumetricIntensity = 0.25f;

    TbdPixelRenderer->AddLight(tempLight);
    TbdPixelRenderer->AddLight(tempLight2);

    /*
    Color tempColor( 226, 230, 179, 255 );
    Vector2 tempAttractPoint{ 100, 50 };
    int numTestMoths = 30;
    for (int i = 0; i < numTestMoths; i++)
    {
        Particle* testParticle = new Particle(Vector2(120,75), Vector2{ -0.8f,-0.25f }, Vector2{ 34.0f,30.0f }, tempColor, Particle_Moth, tempAttractPoint);
        TbdPixelRenderer->particleManager->AddParticle(testParticle);
    }
    */

    Color tempColor{ 141,141,141,255 };
    int numTestDust = 120;
    Vector2 tempRandNum;
    for (int i = 0; i < numTestDust; i++)
    {
        tempRandNum = Vector2{ (float)(rand() % SCREEN_SIZE_X - 10), (float)(rand() % SCREEN_SIZE_Y - 10) };
        tempRandNum += Vector2{10,10};
        Particle* testParticle = new Particle(tempRandNum, Vector2{ -0.8f,-0.25f }, Vector2{ 17.0f, 15.0f }, tempColor, Particle_Dust);
        TbdPixelRenderer->particleManager->AddParticle(testParticle);
    }

    TbdPixelRenderer->GenerateVornoiPoints();

  //  AudioManager.PlayMusic("drips");
    
  //  AudioManager.PlayMusic("forest");

    return Engine::NothingBad;
}

int TbdCanPlaceLight = 0;
bool TbdCanToggleFullBright = true;
bool TbdCanToggleNormalDisplay = true;
bool TbdCanToggleOnlyLights = true;
bool CanPause = true;
bool canToggleScanLines = true;
bool TbdCanRenderWallHitboxes = true;
void TbdPlayerMovement(float dt)
{
    Inputs* inputHandler = Inputs::GetInstance();

    if (inputHandler->keyPressed(SDL_SCANCODE_E) && TbdCanPlaceLight == 1)
    {
        if (TbdPixelRenderer->doBlur == false)
            TbdPixelRenderer->doBlur = true;
        else
            TbdPixelRenderer->doBlur = false;
        TbdCanPlaceLight = 0;
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_E))
    {
        TbdCanPlaceLight = 1;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_GRAVE) && TbdCanToggleFullBright == true)
    {
        if(TbdPixelRenderer->isFullBright == false)
            TbdPixelRenderer->isFullBright = true;
        else
            TbdPixelRenderer->isFullBright = false;
        TbdCanToggleFullBright = false;

        if (!isGravePressedForCheat)
        {
            isGravePressedForCheat = true;
        }
        else
        {
            isGravePressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_GRAVE))
    {
        TbdCanToggleFullBright = true;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_Q) && TbdCanToggleOnlyLights == true)
    {
        if (TbdPixelRenderer->renderOnlyLights == false)
            TbdPixelRenderer->renderOnlyLights = true;
        else
            TbdPixelRenderer->renderOnlyLights = false;
        TbdCanToggleOnlyLights = false;

        if (!isQPressedForCheat)
        {
            isQPressedForCheat = true;
        }
        else
        {
            isQPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_Q))
    {
        TbdCanToggleOnlyLights = true;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_N) && TbdCanToggleNormalDisplay == true)
    {
        if (TbdPixelRenderer->renderNormalMap == false)
            TbdPixelRenderer->renderNormalMap = true;
        else
            TbdPixelRenderer->renderNormalMap = false;
        TbdCanToggleNormalDisplay = false;

        if (!isNPressedForCheat)
        {
            isNPressedForCheat = true;
        }
        else
        {
            isNPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_N))
    {
        TbdCanToggleNormalDisplay = true;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_LSHIFT))
    {
        if (CanPause == true)
        {
            if (Engine::GetInstance()->Paused() == false)
                Engine::GetInstance()->SetPause(true);
            else
                Engine::GetInstance()->SetPause(false);
        }
        CanPause = false;
    }
    else
    {
        CanPause = true;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_C) && TbdCanRenderWallHitboxes == 1)
    {
        if (TbdPixelRenderer->renderWallHitboxes == false)
            TbdPixelRenderer->renderWallHitboxes = true;
        else
            TbdPixelRenderer->renderWallHitboxes = false;
        TbdCanRenderWallHitboxes = 0;

        if (!isCPressedForCheat)
        {
            isCPressedForCheat = true;
        }
        else
        {
            isCPressedForCheat = false;
        }
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_C))
    {
        TbdCanRenderWallHitboxes = 1;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_F) && canToggleScanLines == 1)
    {
        if (TbdPixelRenderer->doScanLines == false)
            TbdPixelRenderer->doScanLines = true;
        else
            TbdPixelRenderer->doScanLines = false;
        canToggleScanLines = 0;
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_F))
    {
        canToggleScanLines = 1;
    }

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
        CursourP *= 1.0f / TbdPixelRenderer->screenScale;
        CursourP += TbdPixelRenderer->GetCameraPosition();

        Vector2 LightP = TbdPixelRenderer->lightSource[0].position;
        Vector2 D = LightP - CursourP;
        float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;
        TbdPixelRenderer->lightSource[0].angle = Angle;

        ImageBuffer* playerEntity = TbdPixelRenderer->animatedObjects[0][0];
        Vector2 ScreenHalfSize = 0.5f * Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
        Vector2 BitmapHalfDim = 0.5f * playerEntity->size;
        TbdPixelRenderer->SetCameraPosition(playerEntity->position - ScreenHalfSize + BitmapHalfDim);
    }
}

void TbdTestScene::Update(float dt)
{
    if (CheckGameScenes() || CheckRestart())
        return;

    TbdPixelRenderer->lightSource[1].position = TbdPixelRenderer->animatedObjects[0][0]->position + Vector2{ 3,3 };
    TbdPixelRenderer->lightSource[0].position = TbdPixelRenderer->animatedObjects[0][0]->position + Vector2{ 3,3 };

    Inputs* inputHandler = Inputs::GetInstance();
    LevelBuilder::GetInstance()->LevelUpdate(dt);
    AudioManager.Update();
    inputHandler->handleInput();

    TbdPlayerMovement(dt);

    ImGuiInterg();
    TbdPixelRenderer->Update(dt);
}

void TbdTestScene::Render()
{
	return;
}

Engine::EngineCode TbdTestScene::Exit()
{
    LevelBuilder::GetInstance()->FreeLevel();
    Inputs::GetInstance()->InputKeyClear();
    TbdPixelRenderer->CleanRenderer();
    SDL_GL_DeleteContext(TbdGlContext);
    //SDL_DestroyWindow(TbdWindow);
    //SDL_Quit();
	return Engine::NothingBad;
}

Engine::EngineCode TbdTestScene::Unload()
{
    delete TbdTestSceneinstance;
    TbdTestSceneinstance = nullptr;
    LevelBuilder::GetInstance()->FreeLevel();
	return Engine::NothingBad;
}

Scene* TbdTestSceneGetInstance(void)
{
    static Scene* TbdSceneinstance = nullptr; // Make it static to ensure a single instance
    if (!TbdTestSceneinstance) {
        TbdTestSceneinstance = new TbdTestScene();
    }
    return TbdTestSceneinstance;
}

/**********************************************************************/

void TbdTestScene::InitImGui()
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsLight();
    ImGui_ImplSDL2_InitForOpenGL(TbdWindow, TbdGlContext);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void TbdTestScene::ImGuiInterg()
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

void TbdTestScene::ImGuiWindow()
{
    if (show_custom_window)
    {
        ImGui::Begin("custom window");
        ImGui::Text("hey how you doin ;)");
        ImGui::Text("welcome to the tbdtestscene debug window");

        int numEntities = LevelBuilder::GetInstance()->CountEntities();
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

        Particle** particles = TbdPixelRenderer->particleManager->GetParticles();

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
            // this crashes :(
            //SceneSystem::GetInstance()->SetScene(TestSceneGetInstance());
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
void TbdTestScene::ImGuiExit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}