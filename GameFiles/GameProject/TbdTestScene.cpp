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
Renderer TbdPixelRenderer;

SDL_Window* TbdWindow;

SDL_GLContext TbdGlContext;

Scene* TbdTestSceneinstance = NULL; 

TbdTestScene::TbdTestScene() : Scene("test")
{

}

Engine::EngineCode TbdTestScene::Load()
{
    AudioManager.LoadMusic("bgm.ogg");
    AudioManager.LoadSFX("footsteps.ogg");
    AudioManager.LoadSFX("oof.ogg");

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
        TbdPixelRenderer.outputBuffer->BufferSizeX * TbdPixelRenderer.outputBuffer->screenScale,
        TbdPixelRenderer.outputBuffer->BufferSizeY * TbdPixelRenderer.outputBuffer->screenScale,
        SDL_WINDOW_OPENGL);
    TbdPixelRenderer.window = TbdWindow;

    TbdGlContext = SDL_GL_CreateContext(TbdWindow);
    SDL_GL_SetSwapInterval(0);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    FileIO::GetInstance()->ReadTileMap("./Data/TileMapSprites.json", &TbdPixelRenderer);
    LevelBuilder::GetInstance()->LoadLevel(&TbdPixelRenderer, "./Data/Tbd_TestLevel.json");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(TbdWindow, TbdGlContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    FileIO::GetInstance()->ReadLight("./Data/TestLight.json", tempLight);

    tempLight2.position.x = 200;
    tempLight2.position.y = 90;

    tempLight2.color = { 216, 247, 255, 255 };

    tempLight2.intensity = 3.5f;
    tempLight2.radius = 75;
    tempLight2.radialFalloff = 4;
    tempLight2.radialWeight = 1;
    tempLight2.angularWeight = 0;
    tempLight2.volumetricIntensity = .25f;

    TbdPixelRenderer.AddLight(tempLight);
    TbdPixelRenderer.AddLight(tempLight2);
    Color tempColor = { 226, 230, 179, 255 };

    int numTestParticles = 30;
    for (int i = 0; i < numTestParticles; i++)
    {
        Particle* testParticle = new Particle(Vector2{ 120, 75 }, Vector2{ -.25f,-.8f }, Vector2{ 0.15f,0.15f }, tempColor, Particle_Moth);
        TbdPixelRenderer.particleManager->AddParticle(testParticle);
    }

    return Engine::NothingBad;
}

int TbdCanPlaceLight = 0;
bool TbdCanToggleFullBright = true;
void TbdPlayerMovement(float dt)
{
    Inputs* inputHandler = Inputs::GetInstance();
   
    if (inputHandler->keyPressed(SDL_SCANCODE_E) && TbdCanPlaceLight == 1)
    {
        //TbdPixelRenderer.AddLight(TbdPixelRenderer.lightSource[0]);
        TbdPixelRenderer.animatedObjects[0][0]->FlipSprite();
        TbdCanPlaceLight = 0;
    }

    if (!inputHandler->keyPressed(SDL_SCANCODE_E))
    {
        TbdCanPlaceLight = 1;
    }
    if (inputHandler->keyPressed(SDL_SCANCODE_GRAVE) && TbdCanToggleFullBright == true)
    {
        if(TbdPixelRenderer.isFullBright == false)
            TbdPixelRenderer.isFullBright = true;
        else
            TbdPixelRenderer.isFullBright = false;
        TbdCanToggleFullBright = false;
    }
    if (!inputHandler->keyPressed(SDL_SCANCODE_GRAVE))
    {
        TbdCanToggleFullBright = true;
    }

    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    Vector2 CursourP = {(float)x, (float)y};
    CursourP *= 1.0f / TbdPixelRenderer.screenScale;
    CursourP += TbdPixelRenderer.GetCameraPosition();

    Vector2 LightP = TbdPixelRenderer.lightSource[0].position;
    Vector2 D = LightP - CursourP;
    float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;
    TbdPixelRenderer.lightSource[0].angle = Angle;
    
	ImageBuffer *playerEntity = TbdPixelRenderer.animatedObjects[0][0];
	Vector2 ScreenHalfSize = 0.5f*Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	Vector2 BitmapHalfDim = 0.5f*playerEntity->size;
	TbdPixelRenderer.SetCameraPosition(playerEntity->position - ScreenHalfSize + BitmapHalfDim);
}

void TbdTestScene::Update(float dt)
{
    if (CheckGameScenes() || CheckRestart())
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();

    Inputs* inputHandler = Inputs::GetInstance();
    LevelBuilder::GetInstance()->LevelUpdate(dt);
    AudioManager.Update();
    inputHandler->handleInput();
    TbdPixelRenderer.UpdateObjects();
    TbdPixelRenderer.UpdateAnimations(dt);

    TbdPlayerMovement(dt);

    if (inputHandler->keyPressed(SDLK_ESCAPE) == true)
    {
        Engine::GetInstance()->SetCloseRequest(true);
    }
}

void TbdTestScene::Render()
{
    TbdPixelRenderer.Update();
	return;
}

Engine::EngineCode TbdTestScene::Exit()
{
    // Remember to clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(TbdGlContext);
    SDL_DestroyWindow(TbdWindow);
    SDL_Quit();
	return Engine::NothingBad;
}

Engine::EngineCode TbdTestScene::Unload()
{
    delete TbdTestSceneinstance;
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
