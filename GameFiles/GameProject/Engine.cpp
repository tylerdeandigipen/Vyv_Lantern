//------------------------------------------------------------------------------
//
// File Name:	BaseSystem.cpp
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		bro.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Engine.h"
#include "BaseSystem.h"
#include "Time.h"
#include <cassert>

// singleton instance
Engine* Engine::instance = new Engine();

Engine::EngineCode Engine::Start()
{
	// time instance
	time = new Time();

	// initalize all systems
	for (int i = 0; i < systemCount; ++i)
	{
		try 
		{
			systems[i]->Init();
		}
		catch (EngineCode engineFailure) 
		{
			assert(engineFailure && "engine failed!");
			return engineFailure;
		}
	}

	// main game loop
	while (isRunning && !closeRequested)
	{
		EngineCode code = NothingBad;

		if (!paused)
		{
			try 
			{
				code = Update();
			}
			catch (EngineCode updateFailure)
			{
				assert(updateFailure && "update failed!");
				return updateFailure;
			}
			if (code == CloseWindow)
				break;
		}

		try 
		{
			code = Render();
		}
		catch (EngineCode renderFailure)
		{
			assert(renderFailure && "render failed!");
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
	catch (EngineCode shutdown) {
		return shutdown;
	}

	return EngineExit;
}

void Engine::EngineAddSystem(BaseSystem* sys)
{
	systems[systemCount++] = sys;
}

bool Engine::Paused() { return paused; }
void Engine::SetPause(bool pause) { paused = pause; }

// get the singleton instance
Engine* Engine::GetInstance() { return instance; }

Engine::Engine() : isRunning(true), systemCount(0), systems(), paused(false), time(NULL)
{

}

Engine::~Engine()
{
	if (instance != 0)
		delete instance;
}

Engine::EngineCode Engine::Update()
{
	float dt = time->Delta();

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
	return EngineExit;
}

// set flag to close game engine for whatever reasons you want
void Engine::SetCloseRequest(bool close)
{
	closeRequested = close;
}
