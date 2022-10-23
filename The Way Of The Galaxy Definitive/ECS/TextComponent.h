#pragma once
#include "Components.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextComponent : public Component {

private:

	PositionComponent* position;
	SDL_RendererFlip flip;
	SDL_Texture* texture;
	SDL_Rect destRect;
	TTF_Font* font;
	std::string text;
	int fontSize;
	SDL_Color color = { 255, 255, 255 };

	// icon variables
	bool icon = false;
	bool iconRight;
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

	TextComponent(std::string mText, const char* mFont, int mFontSize, int r, int g, int b) {

		text = mText;
		fontSize = mFontSize;
		color.r = r; color.g = g; color.b = b;
		font = TTF_OpenFont(mFont, fontSize);
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}

	~TextComponent() {

		SDL_DestroyTexture(texture);
		TTF_CloseFont(font);
		position = nullptr;
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();

		destRect.x = position->position.x;
		destRect.y = position->position.y;
		destRect.h = fontSize * position->scale;
		destRect.w = fontSize * text.size() / 2;

	}

	void update() override {

		destRect.x = position->position.x;
		destRect.y = position->position.y;

		if (icon) {

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

		SDL_DestroyTexture(texture);
		text = mText;
		destRect.w = fontSize * text.size() / 2;
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}

	void setColor(int r, int g, int b) {

		SDL_DestroyTexture(texture);
		color.r = r; color.g = g; color.b = b;
		texture = TextureManager::LoadTexture(font, color, text.c_str());
	}
	
	void addIcon(const char* iconPath, int mIconSize, int mIconSpacing, int mIconMarginTop, bool mIconRight) {

		iconTexture = TextureManager::LoadTexture(iconPath);
		iconSize = 15;
		iconSpacing = mIconSpacing;
		iconMarginTop = mIconMarginTop;
		iconRight = mIconRight;
		
		iconDestRect.w = iconDestRect.h = iconSize;
		icon = true;
	}
};
