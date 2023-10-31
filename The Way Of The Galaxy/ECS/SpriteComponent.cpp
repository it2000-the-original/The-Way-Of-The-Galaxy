#include "ECS/Components.h"
#include "TextureManager.h"
#include "Animation.h"

SpriteComponent::SpriteComponent() {}

SpriteComponent::SpriteComponent(const char* path) {

	texture = TextureManager::LoadTexture(path);
	destroyTexture = true;
}

SpriteComponent::SpriteComponent(SDL_Texture* mTexture) {

	texture = mTexture;
}

void SpriteComponent::init() {

	if (!entity->hasComponent<PositionComponent>()) {

		entity->addComponent<PositionComponent>();
	}

	position = &entity->getComponent<PositionComponent>();

	// Setting source rectangle
	srcRect.x = srcRect.y = 0;
	srcRect.w = position->width;
	srcRect.h = position->height;

	// Setting destination rectangle
	destRect.w = position->width * position->scale;
	destRect.h = position->height * position->scale;
}

void SpriteComponent::update() {

	if (animated) {

		// Updating the animation state

		if (animationSpeed.check()) {

			if (!animationInverted) moveRightFrame();
			else moveDownFrame();
			animationSpeed.init();
		}

		if (!animationInverted) srcRect.y = srcRect.h * animationIndex;
		else srcRect.x = srcRect.w * animationIndex;
	}
}

void SpriteComponent::draw() {

	destRect.x = int(position->position.x);
	destRect.y = int(position->position.y);

	TextureManager::DrawTexture(texture, srcRect, destRect, flip, position->angle);
}

void SpriteComponent::addAnimation(std::string aniName, int f, int i, int s) {

	if (!sourced) {
		Animation ani = Animation(f, i, s);
		animations.emplace(aniName, ani);
	}
}

void SpriteComponent::invertAnimation(bool invert) {

	animationInverted = invert;
}

void SpriteComponent::playAnimation(const char* ani) {

	if (animations.find(ani) != animations.end()) {

		animationFrames = animations[ani].frames;
		animationIndex = animations[ani].index;
		animationSpeed.setDuration(animations[ani].speed);
		animationSpeed.init();
		animated = true;
	}
}

void SpriteComponent::moveRightFrame() {

	// Switch frame of the animation moving right the source position
	if (srcRect.x < srcRect.w * (animationFrames - 1)) srcRect.x += srcRect.w;
	else srcRect.x = 0;
}

void SpriteComponent::moveDownFrame() {

	// Switch frame of the animation moving down the source position
	if (srcRect.y < srcRect.h * (animationFrames - 1)) srcRect.y += srcRect.h;
	else srcRect.y = 0;
}

void SpriteComponent::setFlip(SDL_RendererFlip mFlip) {

	flip = mFlip;
}

void SpriteComponent::setSource(SDL_Rect source) {

	srcRect.x = source.x;
	srcRect.y = source.y;
	bool sourced = true;
}

void SpriteComponent::resetPosition() {

	// This could be usefull if the position
	// is changed after the update of the component

	destRect.x = int(position->position.x);
	destRect.y = int(position->position.y);
}

SpriteComponent::~SpriteComponent() {

	if (destroyTexture)
	SDL_DestroyTexture(texture);
	animations.clear();
}