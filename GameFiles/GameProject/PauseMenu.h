// pausemenu.h

#pragma once

#include <string>

class PauseMenu {
public:
    PauseMenu();
    ~PauseMenu();

    void Render();
    void HandleInput();

    static PauseMenu* GetInstance();

private:
    void RenderButtons();
    void HandleButtonInput();

    bool IsMouseOverBackButton();
    bool IsMouseOverExitButton();
    bool IsMouseOverSettingsButton();

    void HandleBack();
    void HandleExit();
    void HandleSettings();

    static PauseMenu* instance;
};
