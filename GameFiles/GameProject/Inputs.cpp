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
#include "imgui_impl_sdl2.h"

#include "Inputs.h"
#include "Engine.h"

// singleton instance
Inputs* Inputs::instance = new Inputs();

Inputs::Inputs()
{
    quitting = false;
    mouseX = 0;
    mouseY = 0;
    leftMouseB = false;
    rightMouseB = false;
    window = NULL;

    // init key states to false 
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        keyStates[i] = false;
    }
}

// init if a window is passed in
Inputs::Inputs(SDL_Window* window) : Inputs()
{
    this->window = window;
}

Inputs::~Inputs()
{
    delete instance;
}

// get singleotn instance
Inputs* Inputs::GetInstance()
{
    return instance;
}

// this handles all input events 
void Inputs::handleInput()
{
    SDL_Event event;

    // poll event is an event queue for sdl2
    while (SDL_PollEvent(&event))
    {
        // this is necessary so IMGUI doesnt confuse inputs for game
        // vs inputs for IMGUI itself 
#ifdef _DEBUG
        ImGui_ImplSDL2_ProcessEvent(&event);
#endif
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                // sets close request, which shuts down engine
                Engine::GetInstance()->SetCloseRequest(true);
                break;

            case SDL_MOUSEMOTION:
                // detects mouse movement and updates mouse cords
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
                // checks if key is within a valid range
                if (event.key.keysym.scancode >= 0 && event.key.keysym.scancode < SDL_NUM_SCANCODES)
                {
                    // sets those keys to true
                    keyStates[event.key.keysym.scancode] = true;

                    // if escape is pressed, shut down engine
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
                    // sets any keys not pressed to false
                    keyStates[event.key.keysym.scancode] = false;
                }
                break;
            }
        }
    }
}

void Inputs::InputKeyClear()
{
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
        this->keyStates[i] = false;
}

// could be depreciated now, but may have dependancies
bool Inputs::Quitting() const
{
    return quitting;
}

// checks if a specific key is pressed 
bool Inputs::keyPressed(SDL_Keycode key) const
{
    // assert that the key is within the valid range
    assert(key >= 0 && key < SDL_NUM_SCANCODES);

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

// sets the SDL window if needed 
void Inputs::SetWindow(SDL_Window* _window)
{
    window = _window;
}

bool Inputs::mouseButtonPressed(Uint8 button) const
{
    if (button == SDL_BUTTON_LEFT)
    {
        return leftMouseB;
    }
    else if (button == SDL_BUTTON_RIGHT)
    {
        return rightMouseB;
    }

    return false;
}

bool Inputs::keyDown(SDL_Keycode button) const
{
    assert(button >= 0 && button < SDL_NUM_SCANCODES);

    if (button >= 0 && button < SDL_NUM_SCANCODES)
    {
        return keyStates[button];
    }

    return false;
}

bool Inputs::keyUp(SDL_Keycode button) const
{
    assert(button >= 0 && button < SDL_NUM_SCANCODES);

    if (button >= 0 && button < SDL_NUM_SCANCODES)
    {
        return !keyStates[button];
    }

    return false;
}

bool Inputs::mouseButtonDown(SDL_Keycode button) const
{
    if (button == SDL_BUTTON_LEFT)
    {
        return leftMouseB;
    }
    else if (button == SDL_BUTTON_RIGHT)
    {
        return rightMouseB;
    }

    return false;
}

bool Inputs::mouseButtonUp(SDL_Keycode button) const
{
    if (button == SDL_BUTTON_LEFT)
    {
        return !leftMouseB;
    }
    else if (button == SDL_BUTTON_RIGHT)
    {
        return !rightMouseB;
    }

    return false;
}

