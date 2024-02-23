//------------------------------------------------------------------------------
//
// File Name:	PauseMenu.cpp
// Author(s):	Tyler Dean, TayLee Young
// Purpose:     Contains all pause menu code, such as detecting mouse hovering,
//              and button functionality.
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "pausemenu.h"
#include "Engine.h"
#include "Inputs.h"

#include "TbdTestScene.h"

PauseMenu* PauseMenu::instance = new PauseMenu();

PauseMenu::PauseMenu()
{
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::Render()
{
	RenderButtons();
}

void PauseMenu::HandleInput()
{
	HandleButtonInput();
}

void PauseMenu::RenderButtons()
{
}

void PauseMenu::HandleButtonInput()
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

void PauseMenu::HandleBack()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		Engine::GetInstance()->SetPause(false);
	}
}

void PauseMenu::HandleExit()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		Engine::GetInstance()->SetCloseRequest(true);
	}
}

void PauseMenu::HandleSettings()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
	}
}

bool PauseMenu::IsMouseOverBackButton()
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

bool PauseMenu::IsMouseOverExitButton()
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

bool PauseMenu::IsMouseOverSettingsButton()
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

PauseMenu* PauseMenu::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new PauseMenu();
	}
	return instance;
}