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
#include "Engine.h"

Inputs* Inputs::instance = new Inputs();

Inputs::Inputs()
{
    quitting = false;
    mouseX = 0;
    mouseY = 0;
    leftMouseB = false;
    rightMouseB = false;
    window = NULL;

    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        keyStates[i] = false;
    }
}

Inputs::Inputs(SDL_Window* window) : Inputs()
{
    this->window = window;
}

Inputs::~Inputs()
{
    delete instance;
}

Inputs* Inputs::GetInstance()
{
    return instance;
}

void Inputs::handleInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&mouseX, &mouseY);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                rightMouseB = true;
            }
            else if (event.button.button == SDL_BUTTON_LEFT)
            {
                leftMouseB = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                rightMouseB = false;
            }
            else if (event.button.button == SDL_BUTTON_LEFT)
            {
                leftMouseB = false;
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode >= 0 && event.key.keysym.scancode < SDL_NUM_SCANCODES)
            {
                keyStates[event.key.keysym.scancode] = true;

                if (event.key.keysym.sym == SDLK_ESCAPE) 
                {
                    Engine::GetInstance()->SetCloseRequest(true);
                    quitting = true;
                }
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.scancode >= 0 && event.key.keysym.scancode < SDL_NUM_SCANCODES)
            {
                keyStates[event.key.keysym.scancode] = false;
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
    if (key >= 0 && key < SDL_NUM_SCANCODES)
    {
        return keyStates[key];
    }
    return false;
}

int Inputs::getMouseX() const
{
    return mouseX;
}

int Inputs::getMouseY() const
{
    return mouseY;
}

void Inputs::SetWindow(SDL_Window* _window)
{
    window = _window;
}