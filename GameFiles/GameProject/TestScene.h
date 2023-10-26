//------------------------------------------------------------------------------
//
// File Name:	TestScene.h
// Author(s):	TayLee Young
// Purpose:		PLEASE
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "AudioEngine.h"
#include "Collision.h"

class Scene;

struct laser_emitter
{
	Vector2 P;
	Vector2 Direction;
};

struct reflector
{
    Vector2 P;
    Vector2 Direction;
    float Radius;
};

class TestScene : public Scene
{
public:
	
	TestScene();
	~TestScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
private:
	//Renderer pixelRenderer;

#define MAX_LASER_EMITTERS 64
	laser_emitter Emitters[MAX_LASER_EMITTERS];
	uint32_t EmitterCount;

#define MAX_REFLECTORS 64
    reflector Reflectors[MAX_REFLECTORS];
    uint32_t ReflectorCount;
};

Scene* TestSceneGetInstance(void);
