#pragma once
#include "Components.h"
#include "SDL.h"
#include "SDL_image.h"
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
	bool animated;

	// Animation values
	int previousTime;
	int frames;
	int index;
	int speed;

	int var = true;

	std::map<const char*, Animation> animation;

public:
	
	SpriteComponent(const char* path) {

		texture = TextureManager::LoadTexture(path);
		flip = SDL_FLIP_NONE;
		animated = false;
	}

	SpriteComponent(const char* path, bool isAnimated) {

		texture = TextureManager::LoadTexture(path);
		flip = SDL_FLIP_NONE;
		animated = isAnimated;
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

				passNextFrame();
				previousTime = SDL_GetTicks();
			}

		}

		srcRect.y = srcRect.h * index;
		
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

	void passNextFrame() {

		if (srcRect.x < srcRect.w * (frames - 1)) srcRect.x += srcRect.w;
		else srcRect.x = 0;
	}

	void setFlip(SDL_RendererFlip mFlip) {

		flip = mFlip;
	}
};