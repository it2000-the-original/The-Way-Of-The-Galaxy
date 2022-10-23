#pragma once
#include "Components.h"
#include "ColliderIds.h"
#include "GroupLabels.h"
#include "TimeAction.h"
#include <string>

class PlayerComponent : public Component {

private:

	PositionComponent* position;
	SpriteComponent* sprite;

	// This variables define the time before a bullet to another
	const SDL_Rect laserPositionCorrection {10, 6, 0, 0};
	const SDL_Rect missilePositionCorrection {10, 8, 0, 0};
	
	// Bool variables for animations
	TimeAction reactLaserShoot;
	TimeAction reactMissileShoot;
	TimeAction reactShootAnimation  = TimeAction(100);
	TimeAction reactDamageAnimation = TimeAction(100);

public:

	int energy = 100;
	int missiles = 20;
	
	PlayerComponent() {
		
		reactLaserShoot = TimeAction(100);
		reactMissileShoot = TimeAction(300);
	}

	PlayerComponent(int mShootTime) {

		reactLaserShoot = TimeAction(mShootTime);
		reactMissileShoot = TimeAction(mShootTime);
	}

	void init() override {

		position = &entity->getComponent<PositionComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {

		if (reactShootAnimation.check() or reactDamageAnimation.check()) {

			sprite->playAnimation("base");
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
			
		if (reactLaserShoot.check() and !reactDamageAnimation.isActive()) {

			reactLaserShoot.init();
			auto& bullet = entity->manager.addEntity();
			bullet.addComponent<PositionComponent>(position->position.x + position->width * position->scale - laserPositionCorrection.x, position->position.y + position->height * position->scale - laserPositionCorrection.y, 8, 2, 1);
			bullet.getComponent<PositionComponent>().setSpeed(10, 0);
			bullet.addComponent<SpriteComponent>("sprites//lasers//laser.png");
			bullet.addComponent<ColliderComponent>(laserId);
			bullet.addComponent<BulletComponent>();
			bullet.addGroup(groupBullets);
			sprite->playAnimation("shot");
			reactShootAnimation.init();
		}
	}

	void shootMissile() {

		if (reactMissileShoot.check() and !reactDamageAnimation.isActive() and missiles > 0) {

			reactMissileShoot.init();
			auto& bullet = entity->manager.addEntity();
			bullet.addComponent<PositionComponent>(position->position.x + position->width - missilePositionCorrection.x, position->position.y + position->height - missilePositionCorrection.y, 12, 5, 1);
			bullet.getComponent<PositionComponent>().setSpeed(8, 0);
			bullet.addComponent<SpriteComponent>("sprites//missiles//missile.png", true);
			bullet.getComponent<SpriteComponent>().addAnimation("base", 4, 0, 10);
			bullet.getComponent<SpriteComponent>().playAnimation("base");
			bullet.addComponent<ColliderComponent>(missileId);
			bullet.addComponent<BulletComponent>();
			bullet.addComponent<MissileComponent>();
			bullet.addComponent<ExplodeComponent>("sprites//explosions//explosion.png", 50, 50, 40, 60);
			bullet.addGroup(groupBullets);
			sprite->playAnimation("shot");
			reactShootAnimation.init();
			missiles -= 1;
		}
	}

	void reactDamage() {

		reactDamageAnimation = true;
		position->restorePosition(true, true);
		sprite->playAnimation("damage");
		reactDamageAnimation.init();
	}

	~PlayerComponent() {

		position = nullptr;
	}
};