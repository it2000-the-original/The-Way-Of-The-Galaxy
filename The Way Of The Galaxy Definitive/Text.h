#pragma once
#include "Engine.h"

class Text {

private:

	SDL_Texture* texture;
	SDL_Rect destRect;
	TTF_Font* font;
	SDL_Color color;

public:

	Text();
	Text(const char* mFont, int mSize, SDL_Color mColor, const char* text, int x, int y, int w, int h);
	void setText(const char* text);
	void draw();
};
