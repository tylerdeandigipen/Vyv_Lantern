//------------------------------------------------------------------------------
//
// File Name:	PauseMenu.h
// Author(s):	TayLee Young
// Purpose:		Pause class declaration for handling the pause menu.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <memory>
#include "Renderer.h"

class PauseMenu {
public:
	PauseMenu();
	~PauseMenu();

	void Render();
	void HandleInput();

	static PauseMenu* GetInstance();

	bool isPauseMenuOpen;

private:
	void RenderButtons();
	void HandleButtonInput();

	bool IsMouseOverBackButton();
	bool IsMouseOverExitButton();
	bool IsMouseOverExitButtonYes();
	bool IsMouseOverExitButtonNo();
	bool IsMouseOverSettingsButton();
	bool IsMouseOverSettingsExitButton();
	bool IsMouseOverAmbience();
	bool IsMouseOverSFX();
	bool IsMouseOverReset();
	bool IsMouseOverFullscreen();
	bool IsMouseOverHelp();
	bool IsMouseOverCloseHelp();

	void HandleBack();
	void HandleExit();
	void HandleSettings();
	void HandleResetOptions();
	void HandleFullScreen();
	void HandleHelp();

	int CheckAmbienceArea();
	int CheckSFXArea();
	void CloseSettingsMenu();
	void OpenSettingsMenu();
	void OpenExitMenu();
	void CloseExitMenu();
	void OpenHelp();
	void CloseHelp();

	bool settingsMenuOpen;
	bool exitMenuOpen;
	bool audioDirty;
	bool helpOpen;

	static std::unique_ptr<PauseMenu> instance;
};
