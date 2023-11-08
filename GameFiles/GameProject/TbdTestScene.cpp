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
#include "FileIO.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "Inputs.h"
#include "Logging.h"

#include "ImageBuffer.h"
#include "Light.h"
#include "LevelBuilder.h"

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
    TbdWindow = SDL_CreateWindow("MAIN SCENE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        TbdPixelRenderer->outputBuffer->BufferSizeX * TbdPixelRenderer->outputBuffer->screenScale,
        TbdPixelRenderer->outputBuffer->BufferSizeY * TbdPixelRenderer->outputBuffer->screenScale,
        SDL_WINDOW_OPENGL);
    TbdPixelRenderer->window = TbdWindow;

    TbdGlContext = SDL_GL_CreateContext(TbdWindow);
    SDL_GL_SetSwapInterval(0);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    FileIO::GetInstance()->ReadTileSet("./Data/TileMapSprites.json", TbdPixelRenderer);
    FileIO::GetInstance()->ReadTileSet("./Data/TileMapNormals.json", TbdPixelRenderer, true);
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
    tempLight2.volumetricIntensity = .25f;

    //tempLight.intensity = 0;


    TbdPixelRenderer->AddLight(tempLight);
    TbdPixelRenderer->AddLight(tempLight2);

    tempLight2.intensity = 5.0f;
    tempLight2.radius = 25;
    //TbdPixelRenderer->AddLight(tempLight2);

    Color tempColor( 226, 230, 179, 255 );

    int numTestParticles = 30;
    for (int i = 0; i < numTestParticles; i++)
    {
        Particle* testParticle = new Particle(Vector2{ 120, 75 }, Vector2{ -.25f,-.8f }, Vector2{ 0.15f,0.15f }, tempColor, Particle_Moth);
        TbdPixelRenderer->particleManager->AddParticle(testParticle);
    }
    AudioManager.PlayMusic("drips");
    
    AudioManager.PlayMusic("forest");

    return Engine::NothingBad;
}

int TbdCanPlaceLight = 0;
bool TbdCanToggleFullBright = true;
bool TbdCanToggleNormalDisplay = true;
bool TbdCanToggleOnlyLights = true;
void TbdPlayerMovement(float dt)
{
    Inputs* inputHandler = Inputs::GetInstance();

    if (inputHandler->keyPressed(SDL_SCANCODE_E) && TbdCanPlaceLight == 1)
    {
        //TbdPixelRenderer.AddLight(TbdPixelRenderer.lightSource[0]);
        TbdPixelRenderer->animatedObjects[0][0]->FlipSprite();
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
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_N))
    {
        TbdCanToggleNormalDisplay = true;
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

    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    Vector2 CursourP = {(float)x, (float)y};
    CursourP *= 1.0f / TbdPixelRenderer->screenScale;
    CursourP += TbdPixelRenderer->GetCameraPosition();
   // TbdPixelRenderer->lightSource[2].position = CursourP;

    Vector2 LightP = TbdPixelRenderer->lightSource[0].position;
    Vector2 D = LightP - CursourP;
    float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;
    TbdPixelRenderer->lightSource[0].angle = Angle;
    
	ImageBuffer *playerEntity = TbdPixelRenderer->animatedObjects[0][0];
	Vector2 ScreenHalfSize = 0.5f*Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	Vector2 BitmapHalfDim = 0.5f*playerEntity->size;
	TbdPixelRenderer->SetCameraPosition(playerEntity->position - ScreenHalfSize + BitmapHalfDim);
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

    TbdPixelRenderer->UpdateObjects();
    TbdPixelRenderer->UpdateAnimations(dt);
    TbdPixelRenderer->UpdateFace(TbdPixelRenderer->faceState);

    TbdPlayerMovement(dt);
}

void TbdTestScene::Render()
{
    ImGuiInterg();

    TbdPixelRenderer->Update();
	return;
}

Engine::EngineCode TbdTestScene::Exit()
{
    // Remember to clean up
    Inputs::GetInstance()->InputKeyClear();
    TbdPixelRenderer->CleanRenderer();
    SDL_GL_DeleteContext(TbdGlContext);
    SDL_DestroyWindow(TbdWindow);
    SDL_Quit();
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
        ImGui::Text("hey bbg how you doin ;)");

        if (ImGui::Button("Toggle Metrics/Debug Bar"))
        {
            show_metrics_debug_bar = !show_metrics_debug_bar;
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