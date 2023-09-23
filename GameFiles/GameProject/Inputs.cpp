#include <SDL/SDL.h>
#include "Inputs.h"

// for mouse tracking, just throw it in here, and add x and y cords to .h!!!!!!!!! - taylee
void Inputs::handleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			// come back
			quitting = true;
			break;

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
	default:
		return false;
	}
}