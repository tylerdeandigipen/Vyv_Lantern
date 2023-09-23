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
	Inputs();
	~Inputs();

	Inputs(SDL_Window* window);

	void handleInput();

	bool Quitting() const;
	bool keyPressed(SDL_Keycode key) const;

private:
	SDL_Window* window;

	bool quitting;
	bool wKey;
	bool aKey;
	bool sKey;
	bool dKey;
	bool escapeKey;
};
