#pragma once
#include <SDL/SDL.h>
#include <vector>
#include <string>

class DebugVisuals
{
public:
    DebugVisuals(SDL_Renderer* _renderer, int _x, int _y, int _lineSpacing, int _fontSize);
    void AddValue(const std::string& label, int value);
    void Draw();
    void Clear();

private:
    SDL_Renderer* renderer;
    int x, y;
    int lineSpacing;
    int fontSize;
    std::vector<std::string> messages;
};