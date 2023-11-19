// pausemenu.cpp

#include "pausemenu.h"
#include "Engine.h"
#include "Inputs.h"

PauseMenu::PauseMenu(const std::string& filename) 
{

}

PauseMenu::~PauseMenu() 
{

}

void PauseMenu::Render() 
{

    RenderButtons();
}

void PauseMenu::HandleInput() 
{
    HandleButtonInput();
}

void PauseMenu::RenderButtons() {
    // Render your buttons here, you can use the existing code from Renderer::RenderMenu

    // Example button rendering code:
    // Draw a back button
    // Draw an exit button
}

void PauseMenu::HandleButtonInput() 
{
    if (IsMouseOverBackButton()) {
        if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT)) {
            Engine::GetInstance()->SetPause(false);
        }
    }

    if (IsMouseOverExitButton()) {
        if (Inputs::GetInstance()->mouseButtonPressed(SDL_BUTTON_LEFT)) {
            Engine::GetInstance()->SetCloseRequest(true);
        }
    }
}

bool PauseMenu::IsMouseOverBackButton() 
{
    int mouseX = Inputs::GetInstance()->getMouseX();
    int mouseY = Inputs::GetInstance()->getMouseY();

    int backButtonTopLeftX = 40;
    int backButtonTopLeftY = 336;
    int backButtonBottomRightX = 240;
    int backButtonBottomRightY = 440;

    return (mouseX >= backButtonTopLeftX && mouseX <= backButtonBottomRightX &&
        mouseY >= backButtonTopLeftY && mouseY <= backButtonBottomRightY);
}

bool PauseMenu::IsMouseOverExitButton() 
{
    int mouseX = Inputs::GetInstance()->getMouseX();
    int mouseY = Inputs::GetInstance()->getMouseY();

    int exitButtonTopLeftX = 300;
    int exitButtonTopLeftY = 250;
    int exitButtonBottomRightX = 490;
    int exitButtonBottomRightY = 370;

    return (mouseX >= exitButtonTopLeftX && mouseX <= exitButtonBottomRightX &&
        mouseY >= exitButtonTopLeftY && mouseY <= exitButtonBottomRightY);
}