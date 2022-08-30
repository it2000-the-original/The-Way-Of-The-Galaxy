#pragma once
#include "Components.h"
#include "ColliderIds.h"
#include "GroupLabels.h"

class PlayerComponent : public Component {

private:

	PositionComponent* position;
	SpriteComponent* sprite;

	// This variables define the time before a bullet to another
	int shootTime;
	Uint32 initialShootTime;
	const SDL_Rect laserPositionCorrection {10, 6, 0, 0};

	// Bool variables for animations
	bool reactShotAnimation = false;
	Uint32 initialReactShootAnimation;
	Uint32 reactShootAnimationTime = 60;

	bool reactDamageAnimation = false;
	Uint32 initialReactDamageAnimation;
	Uint32 reactDamageAnimationTime = 100;


public:

	int energy = 100;
	
	PlayerComponent() {
		
		shootTime = 100;
	}

	PlayerComponent(int mShootTime) {

		shootTime = mShootTime;
	}

	void init() override {

		position = &entity->getComponent<PositionComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {

		if (reactShotAnimation and SDL_GetTicks() - initialReactShootAnimation > reactShootAnimationTime or
			reactDamageAnimation and SDL_GetTicks() - initialReactDamageAnimation > reactDamageAnimationTime) {

			sprite->playAnimation("base");
			reactShotAnimation = false;
			reactDamageAnimation = false;
		}

		if (energy <= 0) {

			entity->destroy();
		}
	}

	void checkPosition() {

		if (!position->isCompletelyOnRender()[0]) {

			position->restorePosition(true, false);
		}

		if (!position->isCompletelyOnRender()[1]) {

			position->restorePosition(false, true);
		}
	}

	void shot() {
			
		if (SDL_GetTicks() - initialShootTime > shootTime and !reactDamageAnimation) {

			initialShootTime = SDL_GetTicks();
			auto& bullet = entity->manager.addEntity();
			bullet.addComponent<PositionComponent>(position->position.x + position->width - laserPositionCorrection.x, position->position.y + position->height - laserPositionCorrection.y, 8, 2, 1);
			bullet.getComponent<PositionComponent>().setSpeed(10, 0);
			bullet.addComponent<SpriteComponent>("sprites\\lasers\\laser.png");
			bullet.addComponent<ColliderComponent>(laserId);
			bullet.addComponent<BulletComponent>();
			bullet.addGroup(groupBullets);
			sprite->playAnimation("shot");
			reactShotAnimation = true;
			initialReactShootAnimation = SDL_GetTicks();
		}
	}

	void reactDamage() {

		reactDamageAnimation = true;
		position->restorePosition(true, true);
		sprite->playAnimation("damage");
		initialReactDamageAnimation = SDL_GetTicks();
	}

	~PlayerComponent() {

		position = nullptr;
	}
};