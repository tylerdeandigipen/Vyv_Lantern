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
Light tempLight; 
Light tempLight2;
Color white(255,255,255,255);
Color black(0,0,0,255);
Color transparent(0,0,0,0);
Renderer pixelRenderer;
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
    pixelRenderer.Init();


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

    pixelRenderer.lightSource[0] = &tempLight;
    pixelRenderer.numLights += 1;

    pixelRenderer.lightSource[1] = &tempLight2;
    pixelRenderer.numLights += 1;

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
    tempLight.angle -= 2;
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
