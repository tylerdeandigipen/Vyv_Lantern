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
    /* tempLight.position.x = 120;
    tempLight.position.y = 75;
    tempLight.color.r = 255;
    tempLight.color.g = 255;
    tempLight.color.b = 255;
    tempLight.color.a = 1;
    tempLight.minAngle = -45;
    tempLight.maxAngle = 45;
    tempLight.intensity = 2;
    tempLight.radialMult1 = 0.4f;
    tempLight.radialMult2 = 0;
    tempLight.radialWeight = 1;
    tempLight.angularWeight = 1;
    tempLight.volumetricIntensity = 0.5f;
    lightSources[0] = &tempLight; */

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return Engine::SomethingBad;
    }

	return Engine::NothingBad;
}

void render()
{
    //RenderLightingPass(lightBuffer, lightSources, 1);
}

void TestScene::Update(float dt)
{
    //references
    //https://github.com/ScienceDiscoverer/hello_world/blob/master/DirectoOpenGL_Draw_test.cpp
    //https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glDrawPixels.xml
    //https://www.youtube.com/watch?v=WP0-SJHfKjE
    //https://www.geeksforgeeks.org/how-to-setup-opengl-with-visual-studio-2019-on-windows-10/

	dt = 0;
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
