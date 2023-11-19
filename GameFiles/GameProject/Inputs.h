//------------------------------------------------------------------------------
//
// File Name:	Inputs.h
// Author(s):	TayLee Young
// Purpose:		Handles inputs
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#include <SDL/SDL.h>

class Inputs
{
public:
	Inputs();

	Inputs(SDL_Window* window);

	~Inputs();

	static Inputs* GetInstance();

	void handleInput();

	bool Quitting() const;
	bool keyPressed(SDL_Keycode key) const;
	bool mouseButtonPressed(Uint8 button) const;

	int getMouseX() const;
	int getMouseY() const;

	void InputKeyClear();

	void SetWindow(SDL_Window* window);
private:
	static Inputs* instance;

	SDL_Window* window;

	int mouseX;
	int mouseY;

	bool leftMouseB;
	bool rightMouseB;

	bool quitting;
	bool keyStates[SDL_NUM_SCANCODES];
};
