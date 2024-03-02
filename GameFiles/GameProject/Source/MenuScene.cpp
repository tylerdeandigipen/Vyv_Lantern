//------------------------------------------------------------------------------
//
// File Name:	MenuScene.cpp
// Author(s):	
// Purpose:		Change this to whatever you need
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "MenuScene.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Engine.h"
#include <SDL/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <algorithm>

#include "TbdTestScene.h"
#include "LevelCreatorScene.h"
#include "Scene.h"
#include "PlatformSystem.h"
#include "Engine.h"
#include "Entity.h"
#include "EntityContainer.h"
#include "FileIO.h"
#include "SceneSystem.h"
#include "Renderer.h"
#include "Inputs.h"
#include "Logging.h"
#include "Time.h"

#include "ImageBuffer.h"
#include "Light.h"
#include "LevelBuilder.h"
#include "FontSystem.h"

#include "TestScene.h"


SDL_Renderer* MenuRenderer;

Renderer* MenuPixelRenderer = Renderer::GetInstance();


SDL_Window* MenuWindow;

SDL_GLContext MenuGlContext;


Scene* MenuSceneinstance = NULL; //SORRY MIKEY L MEYERS!!!!!!!!!!!!!!!!!!!!!!

MenuScene::MenuScene() : Scene("Menu")
{

}

MenuScene::~MenuScene()
{

}

Engine::EngineCode MenuScene::Init()
{

	Inputs::GetInstance()->SetWindow(MenuWindow);

	MenuWindow = PlatformSystem::GetInstance()->GetWindowHandle();
	MenuPixelRenderer->window = MenuWindow;










	return Engine::NothingBad;
}

Engine::EngineCode MenuScene::Load()
{
	return Engine::NothingBad;
}

void MenuScene::Update(float dt)
{
	dt = dt;
}

void MenuScene::Render()
{
	return;
}

Engine::EngineCode MenuScene::Unload()
{
	return Engine::NothingBad;
}

Engine::EngineCode MenuScene::Exit()
{
	return Engine::NothingBad;
}

Scene* MenuSceneGetInstance(void)
{
	MenuSceneinstance = new MenuScene();
	return MenuSceneinstance;
}




void MenuScene::RenderButtons()
{
}

void MenuScene::HandleButtonInput()
{
	if (IsMouseOverBackButton())
	{
		HandleBack();
	}

	if (IsMouseOverExitButton())
	{
		HandleExit();
	}

	if (IsMouseOverSettingsButton())
	{
		HandleSettings();
	}
}

void MenuScene::HandleBack()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		Engine::GetInstance()->SetPause(false);
	}
}

void MenuScene::HandleExit()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		Engine::GetInstance()->SetCloseRequest(true);
	}
}

void MenuScene::HandleSettings()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
	}
}

bool MenuScene::IsMouseOverBackButton()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int backButtonTopLeftX = 40;
	int backButtonTopLeftY = 336;
	int backButtonBottomRightX = 240;
	int backButtonBottomRightY = 440;

	return (mouseX >= backButtonTopLeftX && mouseX <= backButtonBottomRightX &&
		mouseY >= backButtonTopLeftY && mouseY <= backButtonBottomRightY);
}

bool MenuScene::IsMouseOverExitButton()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int exitButtonTopLeftX = 300;
	int exitButtonTopLeftY = 250;
	int exitButtonBottomRightX = 490;
	int exitButtonBottomRightY = 370;

	return (mouseX >= exitButtonTopLeftX && mouseX <= exitButtonBottomRightX &&
		mouseY >= exitButtonTopLeftY && mouseY <= exitButtonBottomRightY);
}


bool MenuScene::IsMouseOverSettingsButton()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int settingsButtonTopLeftX = 40;
	int settingsButtonTopLeftY = 240;
	int settingsButtonBottomRightX = 240;
	int settingsButtonBottomRightY = 330;

	return mouseX >= settingsButtonTopLeftX && mouseX <= settingsButtonBottomRightX &&
		mouseY >= settingsButtonTopLeftY && mouseY <= settingsButtonBottomRightY;
}