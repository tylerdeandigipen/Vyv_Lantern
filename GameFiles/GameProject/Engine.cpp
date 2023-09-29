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

Engine* Engine::instance = new Engine();

Engine::EngineCode Engine::Start()
{
	time = new Time();
	for (int i = 0; i < systemCount; ++i)
	{
		try 
		{
			systems[i]->Init();
		}
		catch (EngineCode engineFailure) {
			return engineFailure;
		}
	}

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
			return renderFailure;
		}
		if (code == CloseWindow)
			break;
	}

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

Engine* Engine::GetInstance() { return instance; }

// Priv
//------------------------------------------------------------------------------------------//
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
	//dt = clamp(dt, 0.0f, 0.1f);
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
	//DGL_Graphics_StartDrawing();
	for (int i = 0; i < systemCount; ++i)
	{
		systems[i]->Render();
	}
	//DGL_Graphics_FinishDrawing();
	return NothingBad;
}

Engine::EngineCode Engine::ShutDown()
{
	for (int i = systemCount - 1; i >= 0; --i)
	{
		systems[i]->Close();
	}

	delete time;
	return EngineExit;

	// TYLER PLEASE MAKE SURE YOU PUT STUFF TO SHUT DOWN ALL RENDERING/GRAPHICS STUFF IF THIS IS CALLED THANNNKKKSSS - taylee)
}

void Engine::SetCloseRequest(bool close)
{
	closeRequested = close;
}