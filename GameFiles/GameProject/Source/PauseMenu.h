//------------------------------------------------------------------------------
//
// File Name:	PauseMenu.h
// Author(s):	TayLee Young
// Purpose:		Pause class declaration for handling the pause menu.
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <memory>

class PauseMenu {
public:
	PauseMenu();
	~PauseMenu();

	void Render();
	void HandleInput();

	static PauseMenu* GetInstance();

private:
	void RenderButtons();
	void HandleButtonInput();

	bool IsMouseOverBackButton();
	bool IsMouseOverExitButton();
	bool IsMouseOverSettingsButton();

	void HandleBack();
	void HandleExit();
	void HandleSettings();

	static std::unique_ptr<PauseMenu> instance;
};
