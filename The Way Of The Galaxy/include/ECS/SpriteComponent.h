#pragma once
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "TimeAction.h"
#include "ECS.h"

struct Animation;

class PositionComponent;

class SpriteComponent : public Component {

private:

	PositionComponent* position;

	SDL_Texture* texture;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	bool animated = false;
	bool sourced = false;
	bool destroyTexture = false;

	bool animationInverted = false;
	int animationFrames = 1;
	int animationIndex = 0;
	TimeAction animationSpeed;

	std::map<std::string, Animation> animations;

public:

	SpriteComponent();
	SpriteComponent(const char* path);
	SpriteComponent(SDL_Texture* mTexture);

	void init() override;
	void update() override;
	void draw() override;

	void addAnimation(std::string aniName, int f, int i, int s);
	void invertAnimation(bool invert);
	void playAnimation(const char* ani);
	void moveRightFrame();
	void moveDownFrame();
	void setFlip(SDL_RendererFlip mFlip);
	void setSource(SDL_Rect source);
	void resetPosition();

	~SpriteComponent();
};