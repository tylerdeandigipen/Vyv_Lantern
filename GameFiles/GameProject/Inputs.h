//------------------------------------------------------------------------------
//
// File Name:	Inputs.h
// Author(s):	TayLee Young
// Purpose:		Handles inputs
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include <SDL/SDL.h>
#pragma once

class Inputs
{
public:
	Inputs(SDL_Window* window) : window(window) {};

	void handleInput();

	bool Quitting() const;
	bool keyPressed(SDL_Keycode key) const;

private:
	SDL_Window* window;

	bool quitting = false;
	bool wKey = false;
	bool aKey = false;
	bool sKey = false;
	bool dKey = false;
};
