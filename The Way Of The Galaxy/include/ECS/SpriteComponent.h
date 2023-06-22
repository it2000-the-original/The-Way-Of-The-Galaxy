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
	bool destroyTexture = false;

	bool animationVerticalScroll = false;
	int animationFrames = 1;
	int animationIndex = 0;
	TimeAction animationSpeed = TimeAction(100);

	std::map<std::string, Animation> animations;

public:

	SpriteComponent() {}
	
	SpriteComponent(const char* path) {

		texture = TextureManager::LoadTexture(path);
		destroyTexture = true;
	}

	SpriteComponent(SDL_Texture* mTexture) {

		texture = mTexture;
	}
	
	SpriteComponent(const char* path, bool mAnimationVerticalScroll) {

		texture = TextureManager::LoadTexture(path);
		animationVerticalScroll = mAnimationVerticalScroll;
		destroyTexture = true;
	}

	SpriteComponent(SDL_Texture* mTexture, bool mAnimationVerticalScroll) {

		texture = mTexture;
		animationVerticalScroll = mAnimationVerticalScroll;
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

				if (!animationVerticalScroll) moveRightFrame();
				else moveDownFrame();
				animationSpeed.init();
			}

			if (!animationVerticalScroll) srcRect.y = srcRect.h * animationIndex;
			else srcRect.x = srcRect.w * animationIndex;
		}
	}

	void draw() override {

		destRect.x = int(position->position.x);
		destRect.y = int(position->position.y);

		TextureManager::DrawTexture(texture, srcRect, destRect, flip, position->angle);
	}

	void addAnimation(std::string aniName, int f, int i, int s) {

		Animation ani = Animation(f, i, s);
		animations.emplace(aniName, ani);
	}

	void playAnimation(const char* ani) {

		if (animations.find(ani) != animations.end()) {

			animationFrames = animations[ani].frames;
			animationIndex = animations[ani].index;
			animationSpeed.setDuration(animations[ani].speed);
			animationSpeed.init();
			animated = true;
		}
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

		destRect.x = int(position->position.x);
		destRect.y = int(position->position.y);
	}

	~SpriteComponent() {

		if (destroyTexture)
		SDL_DestroyTexture(texture);
		animations.clear();
	}
};