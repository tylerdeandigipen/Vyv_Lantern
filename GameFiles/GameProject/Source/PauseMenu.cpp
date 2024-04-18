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

PauseMenu::PauseMenu() : settingsMenuOpen(false), exitMenuOpen(false), isPauseMenuOpen(false), audioDirty(false)
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

		if (IsMouseOverReset() && settingsMenuOpen)
		{
			HandleResetOptions();
		}

		if (IsMouseOverAmbience() && settingsMenuOpen)
		{
			int whichSlot = CheckAmbienceArea();
			if (whichSlot != 0)
			{
				if (whichSlot == 1)
				{
					AudioManager.SetMusicVolume(0.0);
					audioDirty = true;
				}
				else if (whichSlot == 2)
				{
					AudioManager.SetMusicVolume(0.3);
					audioDirty = true;
				}
				else if (whichSlot == 3)
				{
					AudioManager.SetMusicVolume(0.5);
					audioDirty = true;
				}
				else if (whichSlot == 4)
				{
					AudioManager.SetMusicVolume(0.7);
					audioDirty = true;
				}
				else if (whichSlot == 5)
				{
					AudioManager.SetMusicVolume(1.0);
					audioDirty = true;
				}
				else
				{
					AudioManager.SetMusicVolume(1.0);
				}
			}
		}

		if (IsMouseOverSFX() && settingsMenuOpen)
		{
			int whichSlot = CheckSFXArea();
			if (whichSlot != 0)
			{
				if (whichSlot == 1)
				{
					AudioManager.SetAudioVolume(0.0);
					audioDirty = true;
				}
				else if (whichSlot == 2)
				{
					AudioManager.SetAudioVolume(0.3);
					audioDirty = true;
				}
				else if (whichSlot == 3)
				{
					AudioManager.SetAudioVolume(0.5);
					audioDirty = true;
				}
				else if (whichSlot == 4)
				{
					AudioManager.SetAudioVolume(0.7);
					audioDirty = true;
				}
				else if (whichSlot == 5)
				{
					AudioManager.SetAudioVolume(1.0);
					audioDirty = true;
				}
				else
				{
					AudioManager.SetAudioVolume(1.0);
					audioDirty = true;
				}
			}
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

void PauseMenu::HandleExit()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (!exitMenuOpen)
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			exitMenuOpen = true;
		}
	}
}

void PauseMenu::HandleResetOptions()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (settingsMenuOpen)
		{
			if (audioDirty)
			{
				AudioManager.PlaySFX("buttonFeedback", 0.5);
				audioDirty = false;
				AudioManager.SetAudioVolume(1.0);
				AudioManager.SetMusicVolume(1.0);
			}
		}
	}
}

void PauseMenu::HandleFullScreen()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (settingsMenuOpen)
		{
			// put here
		}
	}
}

void PauseMenu::OpenExitMenu()
{
	Renderer::GetInstance()->LoadMenuPage(Renderer::GetInstance()->exitConfirmIndex);
}

void PauseMenu::CloseExitMenu()
{
	if (IsMouseOverExitButtonYes())
	{
		if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			exitMenuOpen = false;

			Engine::GetInstance()->SetCloseRequest(true);
		}
	}

	if (IsMouseOverExitButtonNo())
	{
		if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			exitMenuOpen = false;
		}
	}
}

void PauseMenu::OpenSettingsMenu()
{
	Renderer::GetInstance()->LoadMenuPage(Renderer::GetInstance()->settingsMenuIndex);
}

void PauseMenu::CloseSettingsMenu()
{
	if (Inputs::GetInstance()->keyPressed(SDLK_ESCAPE))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);

		settingsMenuOpen = false;
	}

	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);

		settingsMenuOpen = false;
	}
}

int PauseMenu::CheckAmbienceArea()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	// Check if mouse is within the ambience area
	if (IsMouseOverAmbience() && Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		// Calculate the distances to each section's center
		std::vector<Vector2> sectionCenters =
		{
			{547, 396}, {634, 396}, {724, 396}, {812, 396}, {900, 396}
		};

		float minDistance = FLT_MAX;
		int closestSection = -1;

		// Iterate through each section's center and find the closest one
		for (int i = 0; i < sectionCenters.size(); ++i)
		{
			float distance = sqrt(pow(mouseX - sectionCenters[i].x, 2) + pow(mouseY - sectionCenters[i].y, 2));
			if (distance < minDistance)
			{
				minDistance = distance;
				closestSection = i + 1; // Sections are indexed starting from 1
			}
		}

		return closestSection;
	}

	return 0;
}

int PauseMenu::CheckSFXArea()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	// Check if mouse is within the ambience area
	if (IsMouseOverSFX() && Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		// Calculate the distances to each section's center
		std::vector<Vector2> sectionCenters =
		{
			{547, 493}, {634, 493}, {724, 493}, {812, 493}, {900, 493}
		};

		float minDistance = FLT_MAX;
		int closestSection = -1;

		// Iterate through each section's center and find the closest one
		for (int i = 0; i < sectionCenters.size(); ++i)
		{
			float distance = sqrt(pow(mouseX - sectionCenters[i].x, 2) + pow(mouseY - sectionCenters[i].y, 2));
			if (distance < minDistance)
			{
				minDistance = distance;
				closestSection = i + 1; // Sections are indexed starting from 1
			}
		}

		return closestSection;
	}

	return 0;
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

bool PauseMenu::IsMouseOverAmbience()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int topLeftX = 539;
	int topLeftY = 375;
	int bottomRightX = 903;
	int bottomRightY = 424;

	return (mouseX >= topLeftX && mouseX <= bottomRightX &&
		mouseY >= topLeftY && mouseY <= bottomRightY);
}

bool PauseMenu::IsMouseOverSFX()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int topLeftX = 539;
	int topLeftY = 470;
	int bottomRightX = 903;
	int bottomRightY = 526;

	return (mouseX >= topLeftX && mouseX <= bottomRightX &&
		mouseY >= topLeftY && mouseY <= bottomRightY);
}

bool PauseMenu::IsMouseOverReset()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int topLeftX = 744;
	int topLeftY = 608;
	int bottomRightX = 802;
	int bottomRightY = 644;

	return (mouseX >= topLeftX && mouseX <= bottomRightX &&
		mouseY >= topLeftY && mouseY <= bottomRightY);
}

bool PauseMenu::IsMouseOverFullscreen()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int topLeftX = 796;
	int topLeftY = 212;
	int bottomRightX = 835;
	int bottomRightY = 244;

	return (mouseX >= topLeftX && mouseX <= bottomRightX &&
		mouseY >= topLeftY && mouseY <= bottomRightY);
}

PauseMenu* PauseMenu::GetInstance()
{
	if (!instance)
	{
		instance.reset(new PauseMenu());
	}
	return instance.get();
}