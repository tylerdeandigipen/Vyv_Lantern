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

	void HandleBack();
	void HandleExit();
	void HandleSettings();
	void CloseSettingsMenu();
	void OpenSettingsMenu();
	void OpenExitMenu();
	void CloseExitMenu();

	bool settingsMenuOpen;
	bool exitMenuOpen;

	static std::unique_ptr<PauseMenu> instance;
};
