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
#include "SceneSystem.h"
#include "Renderer.h"
#include "Inputs.h"

#include "ImageBuffer.h"
#include "Light.h"

#include <SDL/SDL.h>


ImageBuffer* testSprite;
SDL_Renderer* renderer;
Renderer pixelRenderer;

SDL_Window* window = SDL_CreateWindow("Test Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pixelRenderer.outputBuffer->BufferSizeX * pixelRenderer.outputBuffer->screenScale, pixelRenderer.outputBuffer->BufferSizeY * pixelRenderer.outputBuffer->screenScale, 0);
Inputs inputHandler(window);

Scene* TestSceneinstance = NULL; // ITS A GLOBAL VARIABLE CALM DOWN!! SHOW ME ANOTHER WAY AND ITS GONE

TestScene::TestScene() : Scene("test")
{

}

Engine::EngineCode TestScene::Load()
{
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Init()
{
    Light tempLight;
    Light tempLight2;
    Light tempLight3;
    Color white(255, 255, 255, 255);
    Color black(0, 0, 0, 255);
    Color transparent(0, 0, 0, 0);

    SDL_CreateRenderer(window, -1, 0);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    }
    pixelRenderer.renderer = renderer;

    tempLight.position.x = 80;
    tempLight.position.y = 90;

    tempLight.color.r = 0;
    tempLight.color.g = 0;
    tempLight.color.b = 255;
    tempLight.color.a = 255;

    tempLight.maxAngle = 45;
    tempLight.minAngle = -45;
    tempLight.angle = 200;

    tempLight.intensity = 3;
    tempLight.radialMult1 = 0.4f;
    tempLight.radialMult2 = 0.0f;
    tempLight.radialWeight = 1;
    tempLight.angularWeight = 2.0f;
    tempLight.volumetricIntensity = 1;


    tempLight2.position.x = 120;
    tempLight2.position.y = 50;

    tempLight2.color.r = 255;
    tempLight2.color.g = 0;
    tempLight2.color.b = 0;
    tempLight2.color.a = 255;

    tempLight2.maxAngle = 25;
    tempLight2.minAngle = -25;
    tempLight2.angle = 280;

    tempLight2.intensity = 3;
    tempLight2.radialMult1 = 0.4f;
    tempLight2.radialMult2 = 0.0f;
    tempLight2.radialWeight = 1;
    tempLight2.angularWeight = 2.0f;
    tempLight2.volumetricIntensity = 1;

    tempLight3.position.x = 200;
    tempLight3.position.y = 90;

    tempLight3.color.r = 255;
    tempLight3.color.g = 255;
    tempLight3.color.b = 255;
    tempLight3.color.a = 255;

    tempLight3.maxAngle = 360;
    tempLight3.minAngle = -360;
    tempLight3.angle = 0;

    tempLight3.intensity = 3;
    tempLight3.radialMult1 = 0.4f;
    tempLight3.radialMult2 = 0.0f;
    tempLight3.radialWeight = .5f;
    tempLight3.angularWeight = 0;
    tempLight3.volumetricIntensity = 1;

    pixelRenderer.AddLight(tempLight);
    pixelRenderer.AddLight(tempLight2);
    pixelRenderer.AddLight(tempLight3);


    testSprite = new ImageBuffer(30, 30);
    for (int x = 0; x < testSprite->BufferSizeX; ++x)
    {
        for (int y = 0; y < testSprite->BufferSizeY; ++y)
        {
            if (x > 8 && x < testSprite->BufferSizeX - 8 && y > 8 && y < testSprite->BufferSizeY - 8)
            {
                testSprite->buffer[x][y] = transparent;
            }
            else
                testSprite->buffer[x][y] = white;
        }
    }
    testSprite->position = {30, 30};
    pixelRenderer.AddObject(testSprite);

	return Engine::NothingBad;
}

void TestScene::Update(float dt)
{
    pixelRenderer.Update();
    inputHandler.handleInput();
    pixelRenderer.lightSource[0].angle -= 2;
    if (pixelRenderer.objects[0]->position.x < 180)
    {
        pixelRenderer.objects[0]->position.x += 3;
        pixelRenderer.objects[0]->position.y += 1;
    }
    else
    {
        pixelRenderer.objects[0]->position.y = 30;
        pixelRenderer.objects[0]->position.x = 10;
    }
    pixelRenderer.UpdateObjects();


    if (inputHandler.keyPressed(SDLK_ESCAPE) == true)
    {
        TestScene::Exit();
    }
}
void TestScene::Render()
{
	return;
}

Engine::EngineCode TestScene::Exit()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Unload()
{
    delete TestSceneinstance;
	return Engine::NothingBad;
}

Scene* TestSceneGetInstance(void)
{
    TestSceneinstance = new TestScene();
    return TestSceneinstance;
}


//Scene* TestSceneGetInstance() { return &instance->base; }
