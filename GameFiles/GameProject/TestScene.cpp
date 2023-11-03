//------------------------------------------------------------------------------
//
// File Name:	TestScene.cpp
// Author(s):	TayLee Young
// Purpose:		TEST SCENE PLEASE WORK
//
// Copyright  � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <algorithm>

#include "TestScene.h"
#include "Scene.h"
#include "PlatformSystem.h"
#include "Engine.h"
#include "Entity.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "Inputs.h"
#include "Logging.h"

#include "ImageBuffer.h"
#include "Light.h"
#include "LevelBuilder.h"

Logging& logger = Logging::GetInstance("debugLog.log");

ImageBuffer* testSprite;
ImageBuffer* testSprite1;
ImageBuffer* topwall, *bottomwall, *leftwall, *rightwall;

Entity* testEntity;
Entity* jsonEntity;
SDL_Renderer* renderer;

SDL_Window* window;
Renderer &pixelRenderer = *Renderer::GetInstance();

bool canMove = true; // Initialize to allow movement

SDL_GLContext glContext; // OpenGL context for SDL2

Scene* TestSceneinstance = NULL; // ITS A GLOBAL VARIABLE CALM DOWN!! SHOW ME ANOTHER WAY AND ITS GONE

Color white(255, 255, 255, 255);
Color black(0, 0, 0, 255);
Color grey(150, 150, 150, 255);
Color blue(50, 100, 255, 255);

float soundCooldown = 0.0f;

int ObjCount;
const float pushForce = 1.0f;

static bool tabKeyPreviouslyPressed = false;
static bool show_demo_window = false;

/* NOTICE !!!!!!!!!! Feel free to "turn off" debug messages as you please. You can see them in the debugLog.txt in the game files, or in the output tab when debugging. Literally
   you can call for the logger anywhere as long as you get an instance first. Use it in ur functions or something - taylee */

TestScene::TestScene() : Scene("test")
{

}

Engine::EngineCode TestScene::Load()
{
    AudioManager.LoadMusic("bgm.ogg");
    AudioManager.LoadSFX("footsteps.ogg");
    AudioManager.LoadSFX("oof.ogg");

	return Engine::NothingBad;
}

static laser_emitter *
NewEmitter(void)
{
    laser_emitter *Result = NULL;

    emitter_id ResultID = LaserSystem::GetInstance()->CreateEmitter();
    Result = LaserSystem::GetInstance()->GetEmitter(ResultID);
    
    return(Result);
}

static reflector *
NewReflector(void)
{
    reflector *Result = NULL;

    reflector_id ResultID = LaserSystem::GetInstance()->CreateReflector();
    Result = LaserSystem::GetInstance()->GetReflector(ResultID);
    
    return(Result);
}

Engine::EngineCode TestScene::Init()
{
    {
        ControlledEmitter = LaserSystem::GetInstance()->CreateEmitter();

        // @NOTE: Setup lasers
        laser_emitter *TestLaser = LaserSystem::GetInstance()->GetEmitter(ControlledEmitter);
        TestLaser->P = Vector2(40.0f, 60.0f);
        TestLaser->Direction = Vector2::Normalize(Vector2(0.0f, 1.0f));

        laser_emitter *TestLaser1 = NewEmitter();
        TestLaser1->P = Vector2(50.0f, 30.0f);
        TestLaser1->Direction = Vector2::Normalize(Vector2(1.0f, -1.0f));

        reflector *Reflector = NewReflector();
        Reflector->Radius = 10.0f;
        Reflector->P = Vector2(100.0f, 100.0f);
        Reflector->Direction = Vector2::Normalize(Vector2(-1.0f, -1.0f));

        reflector *Reflector1 = NewReflector();
        Reflector1->Radius = 20.0f;
        Reflector1->P = Vector2(100.0f, 30.0f);
        Reflector1->Direction = Vector2::Normalize(Vector2(0.0f, 1.0f));

        reflector *Reflector2 = NewReflector();
        Reflector2->Radius = 100.0f;
        Reflector2->P = Vector2(200.0f, 50.0f);
        Reflector2->Direction = Vector2::Normalize(Vector2(-1.0f, 0.0f));
    }
    
    /*BGM*/
    //AudioManager.PlayMusic("bgm.ogg");

    logger.LogToAll("Timestamped message: %s", "Starting TestScene!!");

    Inputs::GetInstance()->SetWindow(window);

    Light tempLight;
    Light tempLight2;
    Light tempLight3;

    Color transparent(0, 0, 0, 0);

    // Create SDL Window
    window = SDL_CreateWindow("MAIN SCENE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              pixelRenderer.outputBuffer->BufferSizeX * pixelRenderer.outputBuffer->screenScale,
                              pixelRenderer.outputBuffer->BufferSizeY * pixelRenderer.outputBuffer->screenScale,
                              SDL_WINDOW_OPENGL);
    pixelRenderer.window = window;

    
    // Specify Major version and minor version
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0);
    
    gladLoadGLLoader(SDL_GL_GetProcAddress);
	//LevelBuilder::GetInstance()->LoadLevel(&pixelRenderer, "./Data/FirstLevel.json");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");
    /*
	tempLight.Type = LightSourceType_Directional;
	tempLight.position.x = 80;
    tempLight.position.y = 90;
    tempLight.color = { 216, 247, 255, 255 };
    tempLight.maxAngle = 25;
    tempLight.minAngle = -25;
    tempLight.angle = 200;
    tempLight.intensity = 25.0f;
    tempLight.radialWeight = 1;
    tempLight.angularWeight = 2.0f;
    tempLight.volumetricIntensity = .25f;

    tempLight2.position.x = 120;
    tempLight2.position.y = 50;
    tempLight2.color = { 255, 0, 0, 255 };

    tempLight2.intensity = 50;
    tempLight2.radialWeight = 1;
    tempLight2.angularWeight = 2.0f;
    tempLight2.volumetricIntensity = .25f;

    tempLight3.position.x = 200;
    tempLight3.position.y = 90;

    tempLight3.color = { 255, 182, 76, 255 };

    tempLight3.intensity = 50;
    tempLight3.radialWeight = 1;
    tempLight3.angularWeight = 0;
    tempLight3.volumetricIntensity = .25f;

    pixelRenderer.AddLight(tempLight);
    pixelRenderer.AddLight(tempLight2);
    pixelRenderer.AddLight(tempLight3);

    testSprite = new ImageBuffer("./Assets/PPM/Logo.ppm");
    testSprite->type = COLLIDABLE;
    testSprite->position = { 30, 30 };
    testSprite->layer = 1;
    pixelRenderer.AddObject(testSprite);

    //Add another testsprite to check for collision
    testSprite1 = new ImageBuffer(30, 30);
    for (int x = 0; x < testSprite1->BufferSizeX; ++x)
    {
        for (int y = 0; y < testSprite1->BufferSizeY; ++y)
        {
            if (x > 8 && x < testSprite1->BufferSizeX - 8 && y > 8 && y < testSprite1->BufferSizeY - 8)
            {
                testSprite1->SampleColor(x, y) = transparent;
            }
            else
                testSprite1->SampleColor(x, y) = blue;
        }
    }
    testSprite1->position = { 120, 30 };
    testSprite1->layer = 1;
    pixelRenderer.AddObject(testSprite1);
    testSprite1->type = SWITCH;
    //Walls here
    topwall = new ImageBuffer(pixelRenderer.outputBuffer->BufferSizeX * pixelRenderer.outputBuffer->screenScale, 2);
    for (int x = 0; x < topwall->BufferSizeX; ++x)
    {
        for (int y = 0; y < topwall->BufferSizeY; ++y)
        {
            topwall->SampleColor(x, y) = blue;
        }
    }
    topwall->position = { 0, 0 };
    topwall->layer = 1;
    pixelRenderer.AddObject(topwall);
    topwall->type = WALL;

    bottomwall = new ImageBuffer(pixelRenderer.outputBuffer->BufferSizeX * pixelRenderer.outputBuffer->screenScale, 2);
    for (int x = 0; x < bottomwall->BufferSizeX; ++x)
    {
        for (int y = 0; y < bottomwall->BufferSizeY; ++y)
        {
            bottomwall->SampleColor(x, y) = blue;
        }
    }
    bottomwall->position = { 0, -pixelRenderer.outputBuffer->BufferSizeY * pixelRenderer.outputBuffer->screenScale };
    bottomwall->layer = 1;
    pixelRenderer.AddObject(bottomwall);
    bottomwall->type = WALL;


    


    ObjCount = pixelRenderer.returnObjCnt();
    //temp fix to set object[0] to player type
    pixelRenderer.objects[0]->type = PLAYER;
    */
    logger.LogLine("Debug info: Lights, camera, action! (testScene init)");
	return Engine::NothingBad;
}
int canPlaceMoreLight = 0;
float moveSpeed = 20;
void tempPlayerMovementLol(float dt)
{
    Inputs* inputHandler = Inputs::GetInstance();
    if (canMove)
    {
        if (inputHandler->keyPressed(SDL_SCANCODE_W))
        {
            pixelRenderer.objects[2]->position.y -= moveSpeed * dt;

            logger.LogLine("Debug info: Vyv Up pressed.");
            //pixelRenderer.AddLight(pixelRenderer.staticLightSource[0]);
            //AudioManager.PlaySFX("footsteps.ogg");
        }
        if (inputHandler->keyPressed(SDL_SCANCODE_S))
        {
            pixelRenderer.objects[2]->position.y += moveSpeed * dt;

            logger.LogLine("Debug info: Vyv Down pressed.");
            //AudioManager.PlaySFX("footsteps.ogg");
        }
        if (inputHandler->keyPressed(SDL_SCANCODE_D))
        {
            pixelRenderer.objects[2]->position.x += moveSpeed * dt;

            logger.LogLine("Debug info: Vyv Right pressed.");
            //AudioManager.PlaySFX("footsteps.ogg");
        }
        if (inputHandler->keyPressed(SDL_SCANCODE_A))
        {
            pixelRenderer.objects[2]->position.x -= moveSpeed * dt;

            logger.LogLine("Debug info: Vyv Left pressed.");
            //AudioManager.PlaySFX("footsteps.ogg");
        }
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_E) && canPlaceMoreLight == 1)
    {
        pixelRenderer.AddLight(pixelRenderer.lightSource[0]);
        canPlaceMoreLight = 0;
    }

    if (!inputHandler->keyPressed(SDL_SCANCODE_E))
    {
        canPlaceMoreLight = 1;
    }

    if (inputHandler->keyPressed(SDL_SCANCODE_TAB))
    {
        if (!tabKeyPreviouslyPressed)
        {
            show_demo_window = !show_demo_window;
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
    CursourP *= 1.0f / pixelRenderer.screenScale;
    CursourP += pixelRenderer.GetCameraPosition();

    Vector2 LightP = pixelRenderer.lightSource[0].position;
    Vector2 D = LightP - CursourP;
    float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;
    pixelRenderer.lightSource[0].angle = Angle;

    ImageBuffer *playerEntity = pixelRenderer.objects[1];
    Vector2 ScreenHalfSize = 0.5f*Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	Vector2 BitmapHalfDim = 0.5f*playerEntity->size;
	pixelRenderer.SetCameraPosition(playerEntity->position - ScreenHalfSize + BitmapHalfDim);
}

void TestScene::Update(float dt)
{
    if (CheckGameScenes() || CheckRestart())
        return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();

    Inputs* inputHandler = Inputs::GetInstance();
    //LevelBuilder::GetInstance()->LevelUpdate(dt);
    AudioManager.Update();
    inputHandler->handleInput();
    pixelRenderer.UpdateObjects();

    if (inputHandler->keyPressed(SDLK_ESCAPE) == true)
    {
        Engine::GetInstance()->SetCloseRequest(true);
    }
    /*
    //object[2] is the goose
    //object[1] is the square
    for (int i = 0; i < ObjCount; i++)
    {
        pixelRenderer.objects[i]->aabb.min = { pixelRenderer.objects[i]->position.x, pixelRenderer.objects[i]->position.y };
        pixelRenderer.objects[i]->aabb.max = { pixelRenderer.objects[i]->position.x + pixelRenderer.objects[i]->BufferSizeX, pixelRenderer.objects[i]->position.y + pixelRenderer.objects[i]->BufferSizeY };
    };
    */
    /*This will make ALL objects collidable*/
    /*
        for (int b = 1; b < ObjCount; b++)
        {
            if (!CollisionCheck(pixelRenderer.objects[0]->aabb, pixelRenderer.objects[b]->aabb))
            {
                canMove = true;
                tempPlayerMovementLol(dt);
            }
            else
            {
                if (pixelRenderer.objects[b]->type == COLLIDABLE)
                {
                    // Calculate the vector from object 'a' to object 'b'
                    float pushDirX = pixelRenderer.objects[b]->position.x - pixelRenderer.objects[0]->position.x;
                    float pushDirY = pixelRenderer.objects[b]->position.y - pixelRenderer.objects[0]->position.y;
                    // Calculate the length of the vector
                    float pushDirLength = sqrt(pushDirX * pushDirX + pushDirY * pushDirY);

                    // Normalize the vector to obtain a unit vector
                    if (pushDirLength > 0)
                    {
                        pushDirX /= pushDirLength;
                        pushDirY /= pushDirLength;
                    }

                    // Apply the push force to both objects
                    pixelRenderer.objects[0]->position.x -= pushDirX * pushForce;
                    pixelRenderer.objects[0]->position.y -= pushDirY * pushForce;
                    pixelRenderer.objects[b]->position.x += pushDirX * pushForce;
                    pixelRenderer.objects[b]->position.y += pushDirY * pushForce;
                }
                if (pixelRenderer.objects[b]->type == SWITCH)
                {
                    for (int x = 0; x < pixelRenderer.objects[b]->BufferSizeX; ++x)
                    {
                        for (int y = 0; y < pixelRenderer.objects[b]->BufferSizeY; ++y)
                        {
                           pixelRenderer.objects[b]->SampleColor(x, y) = black;
                        }
                    }
                }

                if (pixelRenderer.objects[b]->type == WALL)
                {
                        // Calculate the vector from object 'a' to object 'b'
                        float pushDirX = pixelRenderer.objects[b]->position.x - pixelRenderer.objects[0]->position.x;
                        float pushDirY = pixelRenderer.objects[b]->position.y - pixelRenderer.objects[0]->position.y;

                        // Calculate the length of the vector
                        float pushDirLength = sqrt(pushDirX * pushDirX + pushDirY * pushDirY);

                        // Normalize the vector to obtain a unit vector
                        if (pushDirLength > 0)
                        {
                            pushDirX /= pushDirLength;
                            pushDirY /= pushDirLength;
                        }

                        // Calculate the overlap between the player and the wall's AABB
                        float playerLeft = pixelRenderer.objects[0]->aabb.min.x;
                        float playerRight = pixelRenderer.objects[0]->aabb.max.x;
                        float playerTop = pixelRenderer.objects[0]->aabb.min.y;
                        float playerBottom = pixelRenderer.objects[0]->aabb.max.y;

                        float wallLeft = pixelRenderer.objects[b]->aabb.min.x;
                        float wallRight = pixelRenderer.objects[b]->aabb.max.x;
                        float wallTop = pixelRenderer.objects[b]->aabb.min.y;
                        float wallBottom = pixelRenderer.objects[b]->aabb.max.y;

                        // Check if there is an overlap in both the x and y directions
                        if (playerRight > wallLeft && playerLeft < wallRight && playerBottom > wallTop && playerTop < wallBottom)
                        {
                            // There's a collision with the wall, so prevent further player movement.
                            canMove = false;
                        }
                }



                if (soundCooldown <= 0.0f) 
                {
                    AudioManager.PlaySFX("footsteps.ogg");
                    soundCooldown = 1.0f; // Set the cooldown time
                }
                //AudioManager.PlaySFX("oof.ogg");
            }
        }
    // Update the cooldown timer.
    soundCooldown -= dt;
    if (soundCooldown < 0.0f) {
        soundCooldown = 0.0f;
    }
    */
    {
        int CursourX = 0;
        int CursourY = 0;
        uint32_t Buttons = SDL_GetMouseState(&CursourX, &CursourY);
        
        Vector2 CursourP = Vector2((float)CursourX, (float)CursourY);
        CursourP *= 1.0f / pixelRenderer.screenScale;
        CursourP += pixelRenderer.GetCameraPosition();
        
		laser_emitter* Emitter = LaserSystem::GetInstance()->GetEmitter(ControlledEmitter);

        if(Emitter)
        {
			Vector2 Delta = CursourP - Emitter->P;
			float Angle = atan2f(Delta.y, Delta.x);
			Emitter->Direction = Vector2(cosf(Angle), sinf(Angle));
        }
    }
}

void TestScene::Render()
{
    pixelRenderer.Update();

    //logger.LogLine("Debug info: Things are being rendered. (testScene rendered)");
	return;
}

Engine::EngineCode TestScene::Exit()
{
    // Remember to clean up
    pixelRenderer.CleanRenderer();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    logger.LogLine("Debug info: Bye-Bye! (testScene exit)");
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Unload()
{
    delete TestSceneinstance;
    //delete testEntity;
    //delete jsonEntity;

  //  LevelBuilder::GetInstance()->FreeLevel();

    logger.LogLine("Debug info: entities destroyed :( (testScene unloaded)");
	return Engine::NothingBad;
}

Scene* TestSceneGetInstance(void)
{
    static Scene* TestSceneinstance = nullptr; // Make it static to ensure a single instance
    if (!TestSceneinstance) {
        TestSceneinstance = new TestScene();
    }
    return TestSceneinstance;
}

