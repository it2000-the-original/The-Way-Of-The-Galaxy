#pragma once
#include "Components.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component {

private:

	PositionComponent* position;
	SDL_RendererFlip flip;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	bool animated = false;
	bool animatedReversed = false;

	// Animation values
	int previousTime;
	int frames;
	int index;
	int speed;

	std::map<const char*, Animation> animation;

public:
	
	SpriteComponent(const char* path) {

		texture = TextureManager::LoadTexture(path);
		flip = SDL_FLIP_NONE;
	}

	SpriteComponent(const char* path, bool isAnimated) {

		texture = TextureManager::LoadTexture(path);
		flip = SDL_FLIP_NONE;
		animated = isAnimated;
	}
	
	SpriteComponent(const char* path, bool isAnimated, bool mAnimatedReversed) {

		texture = TextureManager::LoadTexture(path);
		flip = SDL_FLIP_NONE;
		animated = isAnimated;
		animatedReversed = mAnimatedReversed;
		std::cout << animatedReversed << std::endl;
	}

	~SpriteComponent() {

		SDL_DestroyTexture(texture);
		texture = nullptr;
		position = nullptr;
		animation.clear();
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}
		
		frames = 1;
		index = 0;
		speed = 100;
		position = &entity->getComponent<PositionComponent>();
		srcRect.x = srcRect.y = 0;
		srcRect.w = position->width;
		srcRect.h = position->height;
		destRect.w = position->width * position->scale;
		destRect.h = position->height * position->scale;
	}

	void update() override {

		if (animated) {

			if (SDL_GetTicks() - previousTime > speed) {

				if (!animatedReversed) moveRightFrame();
				else moveDownFrame();
				previousTime = SDL_GetTicks();
			}

			if (!animatedReversed) srcRect.y = srcRect.h * index;
			else srcRect.x = srcRect.w * index;
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

		frames = animation[ani].frames;
		index = animation[ani].index;
		speed = animation[ani].speed;
	}

	void moveRightFrame() {

		if (srcRect.x < srcRect.w * (frames - 1)) srcRect.x += srcRect.w;
		else srcRect.x = 0;
	}

	void moveDownFrame() {

		if (srcRect.y < srcRect.h * (frames - 1)) srcRect.y += srcRect.h;
		else srcRect.y = 0;
	}

	void setFlip(SDL_RendererFlip mFlip) {

		flip = mFlip;
	}
};