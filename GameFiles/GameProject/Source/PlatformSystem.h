//------------------------------------------------------------------------------
//
// File Name:	PlatformSystem.h
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// Purpose:		handles systems (thumbs up)
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "BaseSystem.h"
#include <Windows.h>

//#define Name "Lantern"

struct SDL_Window;
typedef void* SDL_GLContext;

class PlatformSystem : public BaseSystem
{
public:
	~PlatformSystem();
	Engine::EngineCode Init();
	void Update(float dt);
	void Render();
	Engine::EngineCode Close();

	//static PlatformSystem* GetInstance(HINSTANCE, bool);
	static PlatformSystem* GetInstance();
	void ChangeTitle(const char* = "Lantern");
	int GetHeight();
	int GetWidth();
	SDL_Window* GetWindowHandle();
private:
	PlatformSystem();

	static std::unique_ptr<PlatformSystem> instance;
	SDL_Window* winHandle;

	SDL_GLContext oglContext;
};

LRESULT CALLBACK PlatformSystemCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);