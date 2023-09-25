//------------------------------------------------------------------------------
//
// File Name:	Inputs.cpp
// Author(s):	TayLee Young
// Purpose:		Figures out what button is being pressed, setting
//              to true or false so yall can use it easier ;)
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include <SDL/SDL.h>
#include "Inputs.h"

Inputs::Inputs()
{
	quitting = false;
	wKey = false;
	aKey = false;
	sKey = false;
	dKey = false;
	escapeKey = false;
	leftMouseB = false;
	rightMouseB = false;
	mouseX = 0;
	mouseY = 0;
	window = NULL;
}


Inputs::Inputs(SDL_Window* window)
{
	this->window = window;
	quitting = false;
	wKey = false;
	aKey = false;
	sKey = false;
	dKey = false;
	escapeKey = false;
	leftMouseB = false;
	rightMouseB = false;
	mouseX = 0;
	mouseY = 0;
}

Inputs::~Inputs()
{

}

// for mouse tracking, just throw it in here, and add x and y cords to .h!!!!!!!!! - taylee
void Inputs::handleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			quitting = true;
		}

		switch (event.type)
		{
			// come back and do shit to close window!!!
			case SDL_QUIT:
				quitting = true;
				break;

			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouseX, &mouseY);
				break;

			/* case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
					case SDL_BUTTON_RIGHT:
						rightMouseB = true;
						break;
					case SDL_BUTTON_LEFT:
						leftMouseB = true;
						break;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_RIGHT:
					rightMouseB = false;
					break;
				case SDL_BUTTON_LEFT:
					leftMouseB = false;
					break;
				}
				break; */

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_w:
						wKey = true;
						break;
					case SDLK_a:
						aKey = true;
						break;
					case SDLK_s:
						sKey = true;
						break;
					case SDLK_d:
						dKey = true;
						break;
					case SDLK_ESCAPE:
						quitting = true;
						escapeKey = true;
						break;
				}
				break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				case SDLK_w:
					wKey = false;
					break;
				case SDLK_a:
					aKey = false;
					break;
				case SDLK_s:
					sKey = false;
					break;
				case SDLK_d:
					dKey = false;
					break;
				case SDLK_ESCAPE:
					escapeKey = false;
					break;
			}
			break;
		}
	}
}

bool Inputs::Quitting() const
{
	return quitting;
}

bool Inputs::keyPressed(SDL_Keycode key) const
{
	switch (key)
	{
	case SDLK_w:
		return wKey;
	case SDLK_a:
		return aKey;
	case SDLK_s:
		return sKey;
	case SDLK_d:
		return dKey;
	case SDLK_ESCAPE:
		return escapeKey;
	default:
		return false;
	}
}

int Inputs::getMouseX() const
{
	return mouseX;
}

int Inputs::getMouseY() const
{
	return mouseY;
}