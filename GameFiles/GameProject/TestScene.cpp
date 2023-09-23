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

ImageBuffer* lightBuffer;
Light* lightSources[1];
Light tempLight; 
Color white;
Color black;
SDL_Window* window;
SDL_Renderer* renderer;

int scaleDifX;
int scaleDifY;
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

    tempLight.color.r = 255;
    tempLight.color.g = 255;
    tempLight.color.b = 255;
    tempLight.color.a = 255;

    tempLight.maxAngle = 45;
    tempLight.minAngle = -45;
    tempLight.angle = 235;

    tempLight.intensity = 2;
    tempLight.radialMult1 = 0.4f;
    tempLight.radialMult2 = 0.0f;
    tempLight.radialWeight = 1;
    tempLight.angularWeight = 2.0f;
    tempLight.volumetricIntensity = 1;

    lightSources[0] = &tempLight; 
    lightBuffer = new ImageBuffer;

    white.r = 255;
    white.g = 255;
    white.b = 255;
    white.a = 255;

    black.r = 0;
    black.g = 0;
    black.b = 0;
    black.a = 255;

    scaleDifX = (int)(lightBuffer->RenderScreenSizeX / lightBuffer->PixelScreenSizeX);
    scaleDifY = (int)(lightBuffer->RenderScreenSizeX / lightBuffer->PixelScreenSizeX);
    SDL_CreateWindowAndRenderer(lightBuffer->RenderScreenSizeX, lightBuffer->RenderScreenSizeY, 0, &window, &renderer);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return Engine::SomethingBad;
    }
	return Engine::NothingBad;
}

void TestScene::Update(float dt)
{
    for (int x = 0; x < lightBuffer->PixelScreenSizeX; ++x)
    {
        for (int y = 0; y < lightBuffer->PixelScreenSizeY; ++y)
        {
            if (x > 40 && x < 70 && y > 30 && y < 60)
            {
                lightBuffer->buffer[x][y] = white;
            }
            else
            {
                lightBuffer->buffer[x][y] = black;
            }
        }
    } 
    RenderLightingPass(lightBuffer, lightSources, 1);
  
    SDL_RenderSetScale(renderer, 4.0f, 4.0f);
    for (int x = 0; x < lightBuffer->PixelScreenSizeX; ++x)
    {
        for (int y = 0; y < lightBuffer->PixelScreenSizeY; ++y)
        {
            SDL_SetRenderDrawColor(renderer, lightBuffer->buffer[x][y].r, lightBuffer->buffer[x][y].g, lightBuffer->buffer[x][y].b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }

    }
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
	tempLight.angle -= 3;
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
