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

PauseMenu::PauseMenu() : settingsMenuOpen(false), exitMenuOpen(false), isPauseMenuOpen(false), audioDirty(false), helpOpen(false)
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

		if (IsMouseOverHelp())
		{
			HandleHelp();
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
		else if (!settingsMenuOpen)
		{
			Renderer::GetInstance()->numMenuSelections = 0;
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
		else if (!settingsMenuOpen)
		{
			Renderer::GetInstance()->numMenuSelections = 0;
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

		if (IsMouseOverCloseHelp() && helpOpen)
		{
			CloseHelp();
		}

		if (settingsMenuOpen)
		{
			OpenSettingsMenu();
		}

		if (exitMenuOpen)
		{
			OpenExitMenu();
		}

		if (helpOpen)
		{
			OpenHelp();
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

void PauseMenu::HandleHelp()
{
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		if (!helpOpen)
		{
			AudioManager.PlaySFX("buttonFeedback", 0.5);
			helpOpen = true;
		}
	}
}

void PauseMenu::OpenHelp()
{
	Renderer::GetInstance()->LoadMenuPage(Renderer::GetInstance()->helpMenuIndex);
}

void PauseMenu::CloseHelp()
{
	if (Inputs::GetInstance()->keyPressed(SDLK_ESCAPE))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);

		helpOpen = false;
	}

	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT))
	{
		AudioManager.PlaySFX("buttonFeedback", 0.5);

		helpOpen = false;
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
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	Vector2 CursorP = { (float)x, (float)y };
	CursorP *= 1.0f / Renderer::GetInstance()->screenScale;

	// Check if mouse is within the ambience area
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT) && settingsMenuOpen)
	{
		// Define the snap positions
		std::vector<Vector2> snapPositions = {
			{88, 64}, {103, 64}, {118, 64}, {133, 64}, {148, 64}
		};

		// Find the closest snap position to the cursor
		float minDistance = FLT_MAX;
		int closestSnapIndex = -1;

		for (int i = 0; i < snapPositions.size(); ++i)
		{
			float distance = std::abs(CursorP.x - snapPositions[i].x);
			if (distance < minDistance)
			{
				minDistance = distance;
				closestSnapIndex = i;
			}
		}

		// Set the position of the menu selection to the closest snap position
		Vector2 pos = snapPositions[closestSnapIndex];

		Renderer::GetInstance()->menuSelectionPos[2] = pos;
		Renderer::GetInstance()->menuSelectionType[2] = Renderer::GetInstance()->Pip;

		Renderer::GetInstance()->numMenuSelections++;

		return closestSnapIndex + 1;
	}

	return 0;
}

int PauseMenu::CheckSFXArea()
{
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	Vector2 CursorP = { (float)x, (float)y };
	CursorP *= 1.0f / Renderer::GetInstance()->screenScale;

	// Check if mouse is within the ambience area
	if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT) && settingsMenuOpen)
	{
		// Define the snap positions
		std::vector<Vector2> snapPositions = {
			{88, 80}, {103, 80}, {118, 80}, {133, 80}, {148, 80}
		};

		// Find the closest snap position to the cursor
		float minDistance = FLT_MAX;
		int closestSnapIndex = -1;

		for (int i = 0; i < snapPositions.size(); ++i)
		{
			float distance = std::abs(CursorP.x - snapPositions[i].x);
			if (distance < minDistance)
			{
				minDistance = distance;
				closestSnapIndex = i;
			}
		}

		// Set the position of the menu selection to the closest snap position
		Vector2 pos = snapPositions[closestSnapIndex];

		Renderer::GetInstance()->menuSelectionPos[3] = pos;
		Renderer::GetInstance()->menuSelectionType[3] = Renderer::GetInstance()->Pip;

		Renderer::GetInstance()->numMenuSelections++;

		return closestSnapIndex + 1;
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

	int backButtonTopLeftX = 56;
	int backButtonTopLeftY = 444;
	int backButtonBottomRightX = 232;
	int backButtonBottomRightY = 532;

	return (mouseX >= backButtonTopLeftX && mouseX <= backButtonBottomRightX &&
		mouseY >= backButtonTopLeftY && mouseY <= backButtonBottomRightY);
}

bool PauseMenu::IsMouseOverHelp()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int backButtonTopLeftX = 50;
	int backButtonTopLeftY = 342;
	int backButtonBottomRightX = 236;
	int backButtonBottomRightY = 427;

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

bool PauseMenu::IsMouseOverCloseHelp()
{
	int mouseX = Inputs::GetInstance()->getMouseX();
	int mouseY = Inputs::GetInstance()->getMouseY();

	int topLeftX = 636;
	int topLeftY = 692;
	int bottomRightX = 800;
	int bottomRightY = 779;

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