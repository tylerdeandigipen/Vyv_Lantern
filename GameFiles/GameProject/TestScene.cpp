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

#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>


ImageBuffer* lightBuffer;
Light* lightSources[1];
Light tempLight;

//TestScene* instance = new TestScene();

//Scene* TestScene::GetInstance() { return &(instance->base); }

Engine::EngineCode TestScene::Load()
{
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Init()
{
    tempLight.position.x = 120;
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
    lightSources[0] = &tempLight;
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

    
    int argc = 1;
    char* argv[1] = { (char*)"Something" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(400, 400);
    glutInitWindowSize(240, 135);
    glutCreateWindow("DirectDraw");

    // register callbacks for winAPI
    glutDisplayFunc(render);
    glutIdleFunc(render); // render scene every frame even when idle
    //glutIgnoreKeyRepeat(1); // ignore annoying windows key repeat delay


    // OpenGL initialisation

    // enter glut event processing infinite loop
    glutMainLoop();
    
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
	return Engine::NothingBad;
}