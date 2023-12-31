//------------------------------------------------------------------------------
//
// File Name:	Engine.h
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		Core of entire engine, updates, renders, initalizes, and closes
//              all systems.
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#ifndef ENGINE_H
#define ENGINE_H

class BaseSystem;
typedef class Time Time;
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
	void SetCloseRequest(bool close);
	
	static Engine* GetInstance();
    
private:
	Engine();
	~Engine();

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	EngineCode Init();
	EngineCode Update();
	EngineCode Render();
	EngineCode ShutDown();


	BaseSystem* systems[MAX_SYSTEMS];
	int systemCount;

	bool isRunning;
	bool paused;
	bool closeRequested;

	static Engine* instance;

	Time* time;
};

#endif // !ENGINE_H
