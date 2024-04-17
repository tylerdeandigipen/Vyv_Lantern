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
#include "Renderer.h"
#include "AudioEngine.h"

#include "TbdTestScene.h"

std::unique_ptr<PauseMenu> PauseMenu::instance = nullptr;

PauseMenu::PauseMenu() : settingsMenuOpen(false), exitMenuOpen(false), isPauseMenuOpen(false)
{
	//AudioManager.LoadMusicFromJSON("./Data/music.json");//line is good
	//AudioManager.LoadSFXFromJSON("./Data/SFX.json");// line is goodplay
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
	if (isPauseMenuOpen)
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

		if (Inputs::GetInstance()->keyPressed(SDLK_ESCAPE) && settingsMenuOpen == true)
		{
			CloseSettingsMenu();
		}

		if (IsMouseOverSettingsExitButton() && settingsMenuOpen == true)
		{
			CloseSettingsMenu();
		}

		if (IsMouseOverExitButtonYes() && exitMenuOpen == true)
		{
			CloseExitMenu();
		}

		if (IsMouseOverExitButtonNo() && exitMenuOpen == true)
		{
			CloseExitMenu();
		}

		if (settingsMenuOpen)
		{
			OpenSettingsMenu();
		}

		if (exitMenuOpen)
		{
			OpenExitMenu();
		}
	}
}

void PauseMenu::HandleBack()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);
		Engine::GetInstance()->SetPause(false);
	}
}

void PauseMenu::HandleExit()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (!exitMenuOpen)
		{
			exitMenuOpen = true;
		}

		//Engine::GetInstance()->SetCloseRequest(true);
	}
}

void PauseMenu::HandleSettings()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		// If the settings menu is closed, open it
		if (!settingsMenuOpen)
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			settingsMenuOpen = true;
		}
	}
}

void PauseMenu::OpenExitMenu()
{
	int exitConfirmIndex = 2;
	Renderer::GetInstance()->LoadMenuPage(exitConfirmIndex);
}

void PauseMenu::CloseExitMenu()
{
	if (IsMouseOverExitButtonYes())
	{
		if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			exitMenuOpen = false;
			Renderer::GetInstance()->LoadMenuPage(-1);

			Engine::GetInstance()->SetCloseRequest(true);
		}
	}

	if (IsMouseOverExitButtonNo())
	{
		if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			exitMenuOpen = false;
			Renderer::GetInstance()->LoadMenuPage(-1);
		}
	}
}

void PauseMenu::OpenSettingsMenu()
{
	int settingsMenuIndex = 1;
	Renderer::GetInstance()->LoadMenuPage(settingsMenuIndex);
}

void PauseMenu::CloseSettingsMenu()
{
	if (Inputs::GetInstance()->keyPressed(SDLK_ESCAPE))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);

		settingsMenuOpen = false;
		Renderer::GetInstance()->LoadMenuPage(-1);
	}

	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);

		settingsMenuOpen = false;
		Renderer::GetInstance()->LoadMenuPage(-1);
	}
}

bool PauseMenu::IsMouseOverSettingsExitButton()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int exitButtonTopLeftX = 531;
	int exitButtonTopLeftY = 710;
	int exitButtonBottomRightX = 684;
	int exitButtonBottomRightY = 794;

	return (mouseX >= exitButtonTopLeftX && mouseX <= exitButtonBottomRightX &&
		mouseY >= exitButtonTopLeftY && mouseY <= exitButtonBottomRightY);
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

bool PauseMenu::IsMouseOverExitButtonYes()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int exitButtonTopLeftX = 527;
	int exitButtonTopLeftY = 468;
	int exitButtonBottomRightX = 684;
	int exitButtonBottomRightY = 560;

	return (mouseX >= exitButtonTopLeftX && mouseX <= exitButtonBottomRightX &&
		mouseY >= exitButtonTopLeftY && mouseY <= exitButtonBottomRightY);
}

bool PauseMenu::IsMouseOverExitButtonNo()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int exitButtonTopLeftX = 758;
	int exitButtonTopLeftY = 470;
	int exitButtonBottomRightX = 910;
	int exitButtonBottomRightY = 556;

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
	if (!instance)
	{
		instance.reset(new PauseMenu());
	}
	return instance.get();
}