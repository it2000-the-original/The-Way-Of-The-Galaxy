#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Components.h"
#include "TextureManager.h"
#include "TimeAction.h"
#include <map>

struct Animation {

	int frames;
	int index;
	int speed;

	Animation() {}
	Animation(int f, int i, int s) {

		frames = f;
		index = i;
		speed = s;
	}
};

class SpriteComponent : public Component {

private:

	PositionComponent* position;

	SDL_Texture* texture;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	bool animated = false;
	bool reversed = false;

	int animationFrames = 1;
	int animationIndex = 0;
	TimeAction animationSpeed = TimeAction(100);

	std::map<const char*, Animation> animation;

public:

	SpriteComponent() {}
	
	SpriteComponent(const char* path) {

		texture = TextureManager::LoadTexture(path);
	}

	SpriteComponent(const char* path, bool mAnimated) {

		texture = TextureManager::LoadTexture(path);
		animated = mAnimated;
	}
	
	SpriteComponent(const char* path, bool mAnimated, bool mReversed) {

		texture = TextureManager::LoadTexture(path);
		reversed = mReversed;
		animated = mAnimated;
	}

	void init() override {

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

	void update() override {

		if (animated) {

			// Updating the animation state

			if (animationSpeed.check()) {

				if (!reversed) moveRightFrame();
				else moveDownFrame();
				animationSpeed.init();
			}

			if (!reversed) srcRect.y = srcRect.h * animationIndex;
			else srcRect.x = srcRect.w * animationIndex;
		}
		
		destRect.x = position->position.x;
		destRect.y = position->position.y;
	}

	void draw() override {

		TextureManager::DrawTexture(texture, srcRect, destRect, flip, position->angle);
	}

	void addAnimation(const char* aniName, int f, int i, int s) {

		Animation ani = Animation(f, i, s);
		animation.emplace(aniName, ani);
	}

	void playAnimation(const char* ani) {

		animationFrames = animation[ani].frames;
		animationIndex = animation[ani].index;

		animationSpeed.setDuration(animation[ani].speed);
		animationSpeed.init();
	}

	void moveRightFrame() {

		// Switch frame of the animation moving right the source position
		if (srcRect.x < srcRect.w * (animationFrames - 1)) srcRect.x += srcRect.w;
		else srcRect.x = 0;
	}

	void moveDownFrame() {

		// Switch frame of the animation moving down the source position
		if (srcRect.y < srcRect.h * (animationFrames - 1)) srcRect.y += srcRect.h;
		else srcRect.y = 0;
	}

	void setFlip(SDL_RendererFlip mFlip) {

		flip = mFlip;
	}

	void resetPosition() {

		// This could be usefull if the position
		// is changed after the update of the component

		destRect.x = position->position.x;
		destRect.y = position->position.y;
	}

	~SpriteComponent() {

		SDL_DestroyTexture(texture);
		texture = nullptr;
		position = nullptr;
		animation.clear();
	}
};