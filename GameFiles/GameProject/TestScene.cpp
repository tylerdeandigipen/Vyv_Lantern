//------------------------------------------------------------------------------
//
// File Name:	TestScene.cpp
// Author(s):	TayLee Young
// Purpose:		TEST SCENE PLEASE WORK
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

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

#include <SDL/SDL.h>
#include <iostream>

Logging& logger = Logging::GetInstance();

ImageBuffer* testSprite;
ImageBuffer* testSprite1;

Entity* testEntity;

SDL_Renderer* renderer;
Renderer pixelRenderer;

SDL_Window* window = SDL_CreateWindow("MAIN SCENE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pixelRenderer.outputBuffer->BufferSizeX * pixelRenderer.outputBuffer->screenScale, pixelRenderer.outputBuffer->BufferSizeY * pixelRenderer.outputBuffer->screenScale, 0);
Inputs* inputHandler;

Scene* TestSceneinstance = NULL; // ITS A GLOBAL VARIABLE CALM DOWN!! SHOW ME ANOTHER WAY AND ITS GONE

Color white(255, 255, 255, 255);
Color black(0, 0, 0, 255);
Color grey(150, 150, 150, 255);
Color blue(50, 100, 255, 255);

float soundCooldown = 0.0f;

int ObjCount;

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

    logger.LogLine("Debug info: Oof heard. (testScene loaded)");
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Init()
{
    /*BGM*/
    //AudioManager.PlayMusic("bgm.ogg");


    inputHandler = new Inputs(window);
    pixelRenderer.window = window;

    testEntity = new Entity("goose2.ppm", window);
    testEntity->SetInputHandler(inputHandler);

    Light tempLight;
    Light tempLight2;
    Light tempLight3;

    Color transparent(0, 0, 0, 0);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    }
    pixelRenderer.renderer = SDL_CreateRenderer(window, -1, 0);

    tempLight.position.x = 80;
    tempLight.position.y = 90;

    tempLight.color = { 216, 247, 255, 255 };

    tempLight.maxAngle = 45;
    tempLight.minAngle = -45;
    tempLight.angle = 200;

    tempLight.intensity = 2.5f;
    tempLight.radialMult1 = 0.4f;
    tempLight.radialMult2 = 0.0f;
    tempLight.radialWeight = 1;
    tempLight.angularWeight = 2.0f;
    tempLight.volumetricIntensity = .25f;
    tempLight.isStatic = 0;


    tempLight2.position.x = 120;
    tempLight2.position.y = 50;

    tempLight2.color = { 255, 182, 76, 255 };

    tempLight2.maxAngle = 25;
    tempLight2.minAngle = -25;
    tempLight2.angle = 280;

    tempLight2.intensity = 3;
    tempLight2.radialMult1 = 0.4f;
    tempLight2.radialMult2 = 0.0f;
    tempLight2.radialWeight = 1;
    tempLight2.angularWeight = 2.0f;
    tempLight2.volumetricIntensity = .25f;
    tempLight2.isStatic = 1;

    tempLight3.position.x = 200;
    tempLight3.position.y = 90;

    tempLight3.color = { 255, 182, 76, 255 };

    tempLight3.maxAngle = 360;
    tempLight3.minAngle = -360;
    tempLight3.angle = 0;

    tempLight3.intensity = 3;
    tempLight3.radialMult1 = 0.2f;
    tempLight3.radialMult2 = 0.0005;
    tempLight3.radialWeight = .3;
    tempLight3.angularWeight = 0;
    tempLight3.volumetricIntensity = .25f;
    tempLight3.isStatic = 1;

    pixelRenderer.AddLight(tempLight);
    pixelRenderer.AddLight(tempLight2);
    pixelRenderer.AddLight(tempLight3);
    testSprite = new ImageBuffer("Logo.ppm");
    testSprite->position = { 30, 30 };
    testSprite->layer = 1;
    pixelRenderer.AddObject(testSprite);

    /*Add another testsprite to check for collision*/
    testSprite1 = new ImageBuffer(30, 30);
    for (int x = 0; x < testSprite1->BufferSizeX; ++x)
    {
        for (int y = 0; y < testSprite1->BufferSizeY; ++y)
        {
            if (x > 8 && x < testSprite1->BufferSizeX - 8 && y > 8 && y < testSprite1->BufferSizeY - 8)
            {
                testSprite1->buffer[x][y] = transparent;
            }
            else
                testSprite1->buffer[x][y] = blue;
        }
    }
    testSprite1->position = { 120, 30 };
    testSprite1->layer = 1;
    pixelRenderer.AddObject(testSprite1);
    
    testEntity->AddToRenderer(&pixelRenderer);

    int tileMapArray[16][9];

    for (int x = 0; x < 16; ++x)
    {
        for (int y = 0; y < 9; ++y)
        {
            if (x == 0 || y == 0 || x == 15 || y == 8)
            {
                tileMapArray[x][y] = 1;
            }
            else
                tileMapArray[x][y] = 0;
        }
    }

    pixelRenderer.MakeTileMap(tileMapArray);

    ObjCount = pixelRenderer.returnObjCnt();

    logger.LogLine("Debug info: Lights, camera, action! (testScene init)");
	return Engine::NothingBad;
}
int canPlaceMoreLight = 0;
float moveSpeed = 20;
void tempPlayerMovementLol(float dt)
{
    if (inputHandler->keyPressed(SDLK_UP) == true)
    {
        pixelRenderer.objects[0]->position.y -= moveSpeed * dt;

        logger.LogLine("Debug info: Vyv Up pressed.");
        //pixelRenderer.AddLight(pixelRenderer.staticLightSource[0]);
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (inputHandler->keyPressed(SDLK_DOWN) == true)
    {
        pixelRenderer.objects[0]->position.y += moveSpeed * dt;

        logger.LogLine("Debug info: Vyv Down pressed.");
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (inputHandler->keyPressed(SDLK_RIGHT) == true)
    {
        pixelRenderer.objects[0]->position.x += moveSpeed * dt;

        logger.LogLine("Debug info: Vyv Right pressed.");
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (inputHandler->keyPressed(SDLK_LEFT) == true)
    {
        pixelRenderer.objects[0]->position.x -= moveSpeed * dt;

        logger.LogLine("Debug info: Vyv Left pressed.");
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (inputHandler->keyPressed(SDLK_e) == true && canPlaceMoreLight == 1)
    {
        pixelRenderer.AddLight(pixelRenderer.lightSource[0]);
        canPlaceMoreLight = 0;
    }
    if (inputHandler->keyPressed(SDLK_e) == false)
    {
        canPlaceMoreLight = 1;
    }

    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);
    // to find angle between worldspace and screenspace take the worldspace coord and multiply by the screen scale in this case 6
    pixelRenderer.lightSource[0].angle = atan2(x - (pixelRenderer.lightSource[0].position.x * 6), y - (pixelRenderer.lightSource[0].position.y * 6)) * 57.295779f;
}

/*algo for brensenham algo*/
void BrandonTurkeyAlgo(float x1, float y1, float x2, float y2, Light clr)
{

}

void TestScene::Update(float dt)
{
    AudioManager.Update();
    testEntity->Update(dt);
    inputHandler->handleInput();
    pixelRenderer.UpdateObjects();

    if (inputHandler->keyPressed(SDLK_ESCAPE) == true)
    {
        Engine::GetInstance()->SetCloseRequest(true);
    }
    /*object[2] is the goose*/
    /*object[1] is the square*/
    for (int i = 0; i < ObjCount; i++)
    {
        pixelRenderer.objects[i]->aabb.min = { pixelRenderer.objects[i]->position.x, pixelRenderer.objects[i]->position.y };
        pixelRenderer.objects[i]->aabb.max = { pixelRenderer.objects[i]->position.x + pixelRenderer.objects[i]->BufferSizeX, pixelRenderer.objects[i]->position.y + pixelRenderer.objects[i]->BufferSizeY };
    };

    /*This will make ALL objects collidable*/
    for (int a = 0; a < ObjCount; a++)
    {
        for (int b = a + 1; b < ObjCount; b++)
        {
            if (!CollisionCheck(pixelRenderer.objects[a]->aabb, pixelRenderer.objects[b]->aabb))
            {
                tempPlayerMovementLol(dt);
            }
            else
            {
                if (soundCooldown <= 0.0f) 
                {
                    AudioManager.PlaySFX("footsteps.ogg");
                    soundCooldown = 1.0f; // Set the cooldown time
                }
                //AudioManager.PlaySFX("oof.ogg");
            }
        }
    }
    logger.LogLine("Debug info: Things are being done. (testScene updated)");

    // Update the cooldown timer.
    soundCooldown -= dt;
    if (soundCooldown < 0.0f) {
        soundCooldown = 0.0f;
    }
}

void TestScene::Render()
{
    pixelRenderer.Update();

    logger.LogLine("Debug info: Things are being rendered. (testScene rendered)");
	return;
}

Engine::EngineCode TestScene::Exit()
{
    SDL_DestroyWindow(window);
    SDL_Quit();

    logger.LogLine("Debug info: Bye-Bye! (testScene exit)");
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Unload()
{
    delete TestSceneinstance;
    delete testEntity;

    logger.LogLine("Debug info: entities destroyed :( (testScene unloaded)");
	return Engine::NothingBad;
}

Scene* TestSceneGetInstance(void)
{
    TestSceneinstance = new TestScene();
    return TestSceneinstance;
}


//Scene* TestSceneGetInstance() { return &instance->base; }
