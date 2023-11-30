#pragma once
#ifndef FONTSYSTEM_H
#define FONTSYSTEM_H

#include <iostream>
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

class FontSystem {
public:
    FontSystem();
    ~FontSystem();

    bool init(const std::string& fontPath, int fontSize);
    void close();

    static void renderText(const std::string& text, int x, int y, SDL_Renderer* renderer);
    void setFontSize(int fontSize);
    void setFontStyle(TTF_Font* font, int style); // e.g., TTF_STYLE_BOLD, TTF_STYLE_ITALIC
    TTF_Font* getFont() const;

private:
    static TTF_Font* font;
    static int fontSize;
    std::unordered_map<int, TTF_Font*> cachedFonts;

    bool loadFont(const std::string& fontPath);
};

#endif // FONTSYSTEM_H