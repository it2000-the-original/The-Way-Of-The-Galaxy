#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "ECS.h"

class PositionComponent;

// A struct created to reduce the
// number of parameters in the constructor

struct Icon {

	const char* texture;
	int size = 32;
	int spacing = 0;
	int margin = 0;
};

class TextComponent : public Component {

private:

	PositionComponent* position;

	SDL_Texture* texture;
	SDL_Rect destRect;

	TTF_Font* font;
	std::string text;
	SDL_Color color = { 255, 255, 255 };

	bool icon = false;
	bool iconRight = false;
	bool destroyFont = true;

	int iconSize;

	int iconSpacing;
	int iconMarginTop;
	SDL_Rect iconDestRect;
	SDL_Texture* iconTexture;

public:

	TextComponent();
	TextComponent(std::string mText, const char* mFont, int mFontSize);
	TextComponent(std::string mText, const char* mFont, int mFontSize, SDL_Color mColor);
	TextComponent(std::string mText, TTF_Font* mFont);
	TextComponent(std::string mText, TTF_Font* mFont, SDL_Color mColor);

	void init() override;
	void update() override;
	void draw() override;

	void setText(std::string mText);
	void setColor(SDL_Color mColor);
	void addIcon(Icon mIcon, bool mIconRight);

	~TextComponent();
};
