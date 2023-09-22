//------------------------------------------------------------------------------
//
// File Name:	BaseSystem.cpp
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		bro.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "BaseSystem.h"
#include "Engine.h"

Engine* Engine::instance = new Engine();

Engine::EngineCode Engine::Start()
{
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

	/*while (true)
	{
		EngineCode code = NothingBad;

		if (!paused)
		{
			try 
			{
				code = Update(float dt);
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
	} */

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
Engine::Engine() : isRunning(false), systemCount(0), systems(), paused(false)
{

}

Engine::~Engine()
{
	if (instance != 0)
		delete instance;
}

Engine::EngineCode Engine::Update(float dt)
{
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

	return EngineExit;

	// TYLER PLEASE MAKE SURE YOU PUT STUFF TO SHUT DOWN ALL RENDERING/GRAPHICS STUFF IF THIS IS CALLED THANNNKKKSSS - taylee)
}