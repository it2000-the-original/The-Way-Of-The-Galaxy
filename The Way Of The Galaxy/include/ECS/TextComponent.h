#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "PositionComponent.h"

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
	int fontSize;
	SDL_Color color = { 255, 255, 255 };

	bool icon = false;
	bool iconRight = false;
	int iconSize;

	int iconSpacing;
	int iconMarginTop;
	SDL_Rect iconDestRect;
	SDL_Texture* iconTexture;

public:

	TextComponent() {}

	TextComponent(std::string mText, const char* mFont, int mFontSize) {

		text = mText;
		fontSize = mFontSize;

		font = TTF_OpenFont(mFont, fontSize);
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}

	TextComponent(std::string mText, const char* mFont, int mFontSize, SDL_Color mColor) {

		text = mText;
		color = mColor;
		fontSize = mFontSize;

		font = TTF_OpenFont(mFont, fontSize);
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();

		// Setting the destination rectangle
		destRect.x = int(position->position.x);
		destRect.y = int(position->position.y);
		destRect.h = fontSize * position->scale;
		destRect.w = fontSize * int(text.size()) / 2;
	}

	void update() override {

		destRect.x = int(position->position.x);
		destRect.y = int(position->position.y);

		if (icon) {

			// Setting the position of the icon

			if (iconRight) iconDestRect.x = destRect.x + iconSpacing;
			else iconDestRect.x = destRect.x - iconSize - iconSpacing;

			iconDestRect.y = destRect.y + iconMarginTop;
		}
	}

	void draw() override {

		TextureManager::DrawTexture(texture, destRect);
		if (icon) TextureManager::DrawTexture(iconTexture, iconDestRect);
	}

	void setText(std::string mText) {

		// Delete from the memory the previous
		// textu of the sprite before creating a new one

		SDL_DestroyTexture(texture);

		text = mText;
		destRect.w = fontSize * int(text.size()) / 2;
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}

	void setColor(SDL_Color mColor) {

		// Delete from the memory the previous
		// textu of the sprite before creating a new one

		SDL_DestroyTexture(texture);

		color = mColor;
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}
	
	void addIcon(Icon mIcon, bool mIconRight) {

		iconTexture = TextureManager::LoadTexture(mIcon.texture);
		iconMarginTop = mIcon.margin;
		iconSpacing = mIcon.spacing;
		iconSize = mIcon.size;
		
		iconDestRect.w = iconSize;
		iconDestRect.h = iconSize;

		iconRight = mIconRight;
		icon = true;
	}
	
	~TextComponent() {

		SDL_DestroyTexture(texture);
		TTF_CloseFont(font);
	}
};
