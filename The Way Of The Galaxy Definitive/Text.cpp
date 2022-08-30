#include "TextureManager.h"
#include "Text.h"

Text::Text() {}

Text::Text(const char* mFont, int mSize, SDL_Color mColor, const char* text, int x, int y, int w, int h) {

	color = mColor;
	font = TTF_OpenFont(mFont, mSize);
	texture = TextureManager::LoadTexture(font, color, text);
	destRect = { x, y, w, h };
}

void Text::setText(const char* text) {

	SDL_DestroyTexture(texture);
	texture = TextureManager::LoadTexture(font, color, text);
}

void Text::draw() {

	TextureManager::DrawTexture(texture, destRect);
}