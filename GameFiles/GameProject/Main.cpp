//------------------------------------------------------------------------------
//
// File Name:	Main.c
// Author(s):	TayLee Young, Doug Schilling (dschilling)
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "framework.h"
#include "Engine.h"
#include "ComponentFactory.h"
#include "EntityFactory.h"
#include "PlatformSystem.h"
#include "SceneSystem.h"
#include "LevelBuilder.h"
#include "Main.h"
#include "Renderer.h"
#include "ImageBuffer.h"
#include "LaserSystem.h"
#include <SDL/SDL.h>

#include <crtdbg.h>

#define MAX_LOADSTRING 100
#define APIENTRY    WINAPI
#define WINAPI __stdcall

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Engine* engine = Engine::GetInstance();
   	engine->EngineAddSystem(LaserSystem::GetInstance());
    engine->EngineAddSystem(LevelBuilder::GetInstance());
    engine->EngineAddSystem(EntityFactory::GetInstance());
    engine->EngineAddSystem(ComponentFactory::GetInstance());
    engine->EngineAddSystem(PlatformSystem::GetInstance());
    engine->EngineAddSystem(SceneSystem::GetInstance());
    
    Engine::EngineCode returnCode = engine->Start(); 

    //_CrtDumpMemoryLeaks();

    switch (returnCode)
    {
    case Engine::NullWindowHandle:
        return 1;
        break;
    default:
        return 0;
        break;
    }

    MSG msg;

    return (int) msg.hwnd;
}
