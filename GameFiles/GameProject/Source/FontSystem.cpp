//------------------------------------------------------------------------------
//
// File Name:	FontSystem.cpp
// Author(s):	Louis Wang
// Purpose:		FontSystem class implementation for handling fonds using
//              SDL_ttf
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "FontSystem.h"

TTF_Font* FontSystem::font = nullptr;
int FontSystem::fontSize = 0;

FontSystem::FontSystem() {

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

void FontSystem::renderText(const std::string& text, int x, int y, SDL_Renderer* renderer, SDL_Color color) {
	SDL_Color textColor = color;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (textSurface) {
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (textTexture) {

			// Render text to the screen
			SDL_Rect renderRect = { x, y, textSurface->w, textSurface->h };
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

void FontSystem::renderMultilineText(const std::string& text, int x, int y, int maxWidth, SDL_Renderer* renderer, SDL_Color textColor) {
	std::istringstream words(text);
	std::string word;
	std::string line;
	int lineHeight = TTF_FontLineSkip(font); // Line height based on the font

	while (words >> word) {

		// Check if adding the next word exceeds the maxWidth
		int wordWidth, wordHeight;
		TTF_SizeText(font, (word + " ").c_str(), &wordWidth, &wordHeight);

		if (x + wordWidth > maxWidth) {

			// Render the line and adjust y position
			renderText(line, x, y, renderer, textColor);
			y += lineHeight; // Move to the next line
			line.clear();
		}

		line += word + " ";
	}

	// Render the last line if there's remaining text
	if (!line.empty()) {
		renderText(line, x, y, renderer, textColor);
	}
}