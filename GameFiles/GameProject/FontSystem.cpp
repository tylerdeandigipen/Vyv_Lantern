#include "FontSystem.h"

TTF_Font* FontSystem::font = nullptr;
int FontSystem::fontSize = 0;

FontSystem::FontSystem(){
    // Initialize SDL_ttf
    TTF_Init();
}

FontSystem::~FontSystem() {
    close();
    // Quit SDL_ttf
    TTF_Quit();
}

bool FontSystem::init(const std::string& fontPath, int fontSize) {
    close(); // Close existing font if initialized

    if (!loadFont(fontPath)) {
        return false;
    }

    this->fontSize = fontSize;
    return true;
}

void FontSystem::close() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    // Clear the cached fonts
    for (auto& pair : cachedFonts) {
        TTF_CloseFont(pair.second);
    }
    cachedFonts.clear();
}

bool FontSystem::loadFont(const std::string& fontPath) {
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        // Handle font loading error
        return false;
    }
    return true;
}

void FontSystem::renderText(const std::string& text, int x, int y, SDL_Renderer* renderer) {
    SDL_Color textColor = {255, 255, 255}; // White color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            // Render text to the screen
            SDL_Rect renderRect = {x, y, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &renderRect);

            // Clean up
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

void FontSystem::setFontSize(int newFontSize) {
    fontSize = newFontSize;
    close();
}

// Example usage: fontSystem.setFontStyle(fontSystem.getFont(), TTF_STYLE_BOLD);
void FontSystem::setFontStyle(TTF_Font* font, int style) {
    TTF_SetFontStyle(font, style);
}

TTF_Font* FontSystem::getFont() const {
    return font;
}
