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
#include <memory>

class Inputs
{
public:

	Inputs(SDL_Window* window);

	~Inputs();

	static Inputs* GetInstance();

	void handleInput();

	bool Quitting() const;
	bool keyPressed(SDL_Keycode key) const;
	bool keyUp(SDL_Keycode key) const;
	bool keyDown(SDL_Keycode key) const;
	bool mouseButtonUp(SDL_Keycode button) const;
	bool mouseButtonDown(SDL_Keycode button) const;
	bool mouseButtonPressed(Uint8 button) const;
	bool leftMouseButtonClicked();
	bool rightMouseButtonClicked();

	int getMouseX() const;
	int getMouseY() const;

	void InputKeyClear();

	void SetWindow(SDL_Window* window);
private:
	Inputs();
	static std::unique_ptr<Inputs> instance;
	SDL_Window* window;

	int mouseX;
	int mouseY;

	bool leftMouseB;
	bool rightMouseB;
	bool leftMouseButtonPressed;
	bool rightMouseButtonPressed;

	bool quitting;
	bool keyStates[SDL_NUM_SCANCODES];
};
