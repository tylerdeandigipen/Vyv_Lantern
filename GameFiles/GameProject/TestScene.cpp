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

#include "ImageBuffer.h"
#include "Light.h"

#include <SDL/SDL.h>

#define MAX_LIGHT_SOURCES 20

ImageBuffer* lightBuffer;
ImageBuffer* renderBuffer;
ImageBuffer* testSprite;
Light* lightSources[MAX_LIGHT_SOURCES];
Light tempLight; 
Light tempLight2;
Color white(255,255,255,255);
Color black(0,0,0,255);
Color transparent(0,0,0,0);
SDL_Window* window;
SDL_Renderer* renderer;

//TestScene::TestScene(Scene _base) : base(_base)
//{
//}



Scene* instance = NULL; // ITS A GLOBAL VARIABLE CALM DOWN!! SHOW ME ANOTHER WAY AND ITS GONE

TestScene::TestScene() : Scene("test")
{

}

Engine::EngineCode TestScene::Load()
{
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Init()
{
    tempLight.position.x = 80;
    tempLight.position.y = 90;

    tempLight.color.r = 0;
    tempLight.color.g = 0;
    tempLight.color.b = 255;
    tempLight.color.a = 255;

    tempLight.maxAngle = 45;
    tempLight.minAngle = -45;
    tempLight.angle = 200;

    tempLight.intensity = 2;
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

    tempLight2.intensity = 2;
    tempLight2.radialMult1 = 0.4f;
    tempLight2.radialMult2 = 0.0f;
    tempLight2.radialWeight = 1;
    tempLight2.angularWeight = 2.0f;
    tempLight2.volumetricIntensity = 1;

    lightSources[0] = &tempLight; 
    lightSources[1] = &tempLight2;

    lightBuffer = new ImageBuffer;
    renderBuffer = new ImageBuffer;
    testSprite = new ImageBuffer(30,30);

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

    renderBuffer->screenScale = 6;
    SDL_CreateWindowAndRenderer(renderBuffer->BufferSizeX * renderBuffer->screenScale, renderBuffer->BufferSizeY * renderBuffer->screenScale, 0, &window, &renderer);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return Engine::SomethingBad;
    }
	return Engine::NothingBad;
}

void TestScene::Update(float dt)
{
    lightBuffer->AddSprite(testSprite, 30, 30);
    RenderLightingPass(renderBuffer, lightBuffer, lightSources, 2);
  
    SDL_RenderSetScale(renderer, renderBuffer->screenScale, renderBuffer->screenScale);
    for (int x = 0; x < renderBuffer->BufferSizeX; ++x)
    {
        for (int y = 0; y < renderBuffer->BufferSizeY; ++y)
        {
            SDL_SetRenderDrawColor(renderer, renderBuffer->buffer[x][y].r, renderBuffer->buffer[x][y].g, renderBuffer->buffer[x][y].b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    renderBuffer->ClearImageBuffer();
    SDL_RenderPresent(renderer);
    //tempLight.angle -= 2;
}
void TestScene::Render()
{
	return;
}

Engine::EngineCode TestScene::Exit()
{
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Unload()
{
    delete instance;
	return Engine::NothingBad;
}

Scene* TestSceneGetInstance(void)
{
    instance = new TestScene();
    return instance;
}


//Scene* TestSceneGetInstance() { return &instance->base; }
