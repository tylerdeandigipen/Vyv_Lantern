//------------------------------------------------------------------------------
//
// File Name:	Engine.cpp
// Author(s):	TayLee Young
// Purpose:		Core of entire engine, updates, renders, initalizes, and closes
//              all systems.
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Engine.h"
#include "BaseSystem.h"
#include "Time_TM.h"
#include "Renderer.h"
#include "AudioEngine.h"
#include <cassert>

// singleton instance
Engine* Engine::instance = new Engine();

Engine::EngineCode Engine::Start()
{
	// time instance
	time = new Time();

	// will see if still crashes but should be an "okay" place to load sound since it gets deletes on this files exit
	AudioManager.LoadMusicFromJSON("./Data/music.json");//line is good
	AudioManager.LoadSFXFromJSON("./Data/SFX.json");// line is goodplay
	// initalize all systems
	for (int i = 0; i < systemCount; ++i)
	{
		try
		{
			systems[i]->Init();
		}
		catch (EngineCode engineFailure)
		{
			assert(engineFailure && "Engine failed to initialize. Location: Engine::Start(), Init()");
			return engineFailure;
		}
	}

	// main game loop
	while (isRunning && !closeRequested)
	{
		EngineCode code = NothingBad;

		try
		{
			code = Update();
		}
		catch (EngineCode updateFailure)
		{
			assert(updateFailure && "Engine failed during update. Location: Engine::Start(), Update()");
			return updateFailure;
		}
		if (code == CloseWindow)
			break;

		try
		{
			code = Render();
		}
		catch (EngineCode renderFailure)
		{
			assert(renderFailure && "Engine failed during render. Location: Engine::Start(), Render()");
			return renderFailure;
		}
		if (code == CloseWindow)
			break;
	}

	// stop engine
	return Stop();
}

Engine::EngineCode Engine::Stop()
{
	try
	{
		ShutDown();
	}
	catch (EngineCode shutdown)
	{
		assert(shutdown && "Engine failed to shut down. Location: Engine::Stop(), ShutDown()");
		return shutdown;
	}

	return EngineExit;
}

void Engine::EngineAddSystem(BaseSystem* sys)
{
	systems[systemCount++] = sys;
}

bool Engine::Paused()
{
	return paused;
}

void Engine::SetPause(bool pause)
{
	paused = pause;
}

// get the singleton instance
Engine* Engine::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Engine();
	}
	return instance;
}

Engine::Engine() : isRunning(true), systemCount(0), systems(), paused(false), time(NULL), closeRequested(false)
{
}

Engine::~Engine()
{
	if (instance != NULL)
	{
		delete instance;
	}
}

Engine::EngineCode Engine::Update()
{
	float dt = Time::Instance().Delta();

	// update all systems
	for (int i = 0; i < systemCount; ++i)
	{
		try {
			systems[i]->Update(dt);
		}
		catch (EngineCode updateFailure)
		{
			switch (updateFailure)
			{
			case CloseWindow:
				return updateFailure;
			default:
				assert(updateFailure && "Engine failed during system update. Location: Engine::Update()");
				throw(updateFailure);
			}
		}
	}
	return NothingBad;
}

Engine::EngineCode Engine::Render()
{
	// render all systems
	for (int i = 0; i < systemCount; ++i)
	{
		systems[i]->Render();
	}
	return NothingBad;
}

Engine::EngineCode Engine::ShutDown()
{
	// close all systems
	for (int i = systemCount - 1; i >= 0; --i)
	{
		systems[i]->Close();
	}

	delete time;
	AudioManager.Free();
	return EngineExit;
}

// set flag to close game engine for whatever reasons you want
void Engine::SetCloseRequest(bool close)
{
	closeRequested = close;
}