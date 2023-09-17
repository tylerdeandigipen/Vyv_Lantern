//------------------------------------------------------------------------------
//
// File Name:	Engine.cpp
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		bro.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

class BaseSystem;

// to be safe, may change later on - taylee
#define MAX_SYSTEMS 10 

class Engine
{
public:
	enum EngineCode { NullWindowHandle, WindowDoesntExist, AllScenesNull, CloseWindow, SomethingBad, NothingBad, EngineExit };

	EngineCode Start();
	EngineCode Stop();

	void EngineAddSystem(BaseSystem*);

	bool Paused();
	void SetPause(bool pause);
	
	static Engine* GetInstance();

private:
	Engine();
	~Engine();

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	EngineCode Init();
	EngineCode Update(float dt);
	EngineCode Render();
	EngineCode ShutDown();

	BaseSystem* systems[MAX_SYSTEMS];
	int systemCount;

	bool isRunning;
	bool paused;

	static Engine* instance;
};