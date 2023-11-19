// pausemenu.h

#pragma once

#include <string>

class PauseMenu {
public:
    PauseMenu(const std::string& filename);
    ~PauseMenu();

    void Render();
    void HandleInput();

private:
    void RenderButtons();
    void HandleButtonInput();

    bool IsMouseOverBackButton();
    bool IsMouseOverExitButton();
};
