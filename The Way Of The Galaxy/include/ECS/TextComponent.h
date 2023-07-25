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

	TextComponent() {}

	TextComponent(std::string mText, const char* mFont, int mFontSize) {

		text = mText;
		font = TTF_OpenFont(mFont, mFontSize);
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}

	TextComponent(std::string mText, const char* mFont, int mFontSize, SDL_Color mColor) {

		text = mText;
		color = mColor;
		font = TTF_OpenFont(mFont, mFontSize);
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}

	TextComponent(std::string mText, TTF_Font* mFont) {

		text = mText;
		font = mFont;
		destroyFont = false;
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}

	TextComponent(std::string mText, TTF_Font* mFont, SDL_Color mColor) {

		text = mText;
		font = mFont;
		color = mColor;
		destroyFont = false;
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

		TTF_SizeText(font, text.c_str(), &destRect.w, &destRect.h);
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
		TTF_SizeText(font, text.c_str(), &destRect.w, &destRect.h);
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
		if (destroyFont) TTF_CloseFont(font);
	}
};
