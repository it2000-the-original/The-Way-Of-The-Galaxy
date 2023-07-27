#include "TextureManager.h"
#include "ECS/Components.h"

TextComponent::TextComponent() {}

TextComponent::TextComponent(std::string mText, const char* mFont, int mFontSize) {

	text = mText;
	font = TTF_OpenFont(mFont, mFontSize);
	texture = TextureManager::LoadTexture(font, color, text.c_str());
}

TextComponent::TextComponent(std::string mText, const char* mFont, int mFontSize, SDL_Color mColor) {

	text = mText;
	color = mColor;
	font = TTF_OpenFont(mFont, mFontSize);
	texture = TextureManager::LoadTexture(font, color, text.c_str());
}

TextComponent::TextComponent(std::string mText, TTF_Font* mFont) {

	text = mText;
	font = mFont;
	destroyFont = false;
	texture = TextureManager::LoadTexture(font, color, text.c_str());
}

TextComponent::TextComponent(std::string mText, TTF_Font* mFont, SDL_Color mColor) {

	text = mText;
	font = mFont;
	color = mColor;
	destroyFont = false;
	texture = TextureManager::LoadTexture(font, color, text.c_str());
}

void TextComponent::init() {

	if (!entity->hasComponent<PositionComponent>()) {

		entity->addComponent<PositionComponent>();
	}

	position = &entity->getComponent<PositionComponent>();

	// Setting the destination rectangle
	destRect.x = int(position->position.x);
	destRect.y = int(position->position.y);

	TTF_SizeText(font, text.c_str(), &destRect.w, &destRect.h);
}

void TextComponent::update() {

	destRect.x = int(position->position.x);
	destRect.y = int(position->position.y);

	if (icon) {

		// Setting the position of the icon

		if (iconRight) iconDestRect.x = destRect.x + iconSpacing;
		else iconDestRect.x = destRect.x - iconSize - iconSpacing;

		iconDestRect.y = destRect.y + iconMarginTop;
	}
}

void TextComponent::draw() {

	TextureManager::DrawTexture(texture, destRect);
	if (icon) TextureManager::DrawTexture(iconTexture, iconDestRect);
}

void TextComponent::setText(std::string mText) {

	// Delete from the memory the previous
	// textu of the sprite before creating a new one

	SDL_DestroyTexture(texture);

	text = mText;
	TTF_SizeText(font, text.c_str(), &destRect.w, &destRect.h);
	texture = TextureManager::LoadTexture(font, color, text.c_str());
}

void TextComponent::setColor(SDL_Color mColor) {

	// Delete from the memory the previous
	// textu of the sprite before creating a new one

	SDL_DestroyTexture(texture);

	color = mColor;
	texture = TextureManager::LoadTexture(font, color, text.c_str());
}

void TextComponent::addIcon(Icon mIcon, bool mIconRight) {

	iconTexture = TextureManager::LoadTexture(mIcon.texture);
	iconMarginTop = mIcon.margin;
	iconSpacing = mIcon.spacing;
	iconSize = mIcon.size;

	iconDestRect.w = iconSize;
	iconDestRect.h = iconSize;

	iconRight = mIconRight;
	icon = true;
}

TextComponent::~TextComponent() {

	SDL_DestroyTexture(texture);
	if (destroyFont) TTF_CloseFont(font);
}