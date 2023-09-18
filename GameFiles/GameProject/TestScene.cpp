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
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>

//TestScene* instance = new TestScene();

//Scene* TestScene::GetInstance() { return &(instance->base); }

Engine::EngineCode TestScene::Load()
{
	return Engine::NothingBad;
}

Engine::EngineCode TestScene::Init()
{
	return Engine::NothingBad;
}

void TestScene::Update(float dt)
{

    int argc = 1;
    char* argv[1] = { (char*)"Something" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(400, 400);
    glutInitWindowSize(240, 135);
    glutCreateWindow("DirectDraw");

    ImageBuffer* lightBuffer;
    Light *lightSources[1];
    Light tempLight;
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
    RenderLightingPass(lightBuffer, lightSources, 1);
    // register callbacks for winAPI
    glutDisplayFunc(renderWorld);
    glutIdleFunc(renderWorld); // render scene every frame even when idle
    //glutIgnoreKeyRepeat(1); // ignore annoying windows key repeat delay

    // user input callbacks
    glutKeyboardFunc(asciiKeyPressed); // process "normal" keys
    //glutSpecialFunc(specKeyPressed); // process "special" keys
    //glutSpecialUpFunc(specKeyReleased); // check if "special" key is released
    //glutMouseFunc(mouseButton); // register mouse buttons press
    //glutMotionFunc(mouseMove); // register mouse movement with pressed button

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