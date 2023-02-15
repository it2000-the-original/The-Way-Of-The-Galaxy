#pragma once
#include "Components.h"
#include "TimeAction.h"
#include <string>

class PlayerSpaceship : public Component {

private:

	PositionComponent* position;
	SpriteComponent* sprite;

	const SDL_Rect laserPositionCorrection {10, 6, 0, 0};
	const SDL_Rect missilePositionCorrection {10, 8, 0, 0};
	
	TimeAction reactLaserShoot;
	TimeAction reactMissileShoot;
	TimeAction reactShootAnimation  = TimeAction(100);
	TimeAction reactDamageAnimation = TimeAction(100);

	std::string weapons[2] = { "laser", "missile" };
	int SelectedWeapon = 0;

	bool automaticWeapon = false;
	bool alreadySwitched = false;

public:

	int energy = 100;
	int missiles = 20;
	
	PlayerSpaceship() {
		
		reactLaserShoot = TimeAction(100);
		reactMissileShoot = TimeAction(300);
	}

	PlayerSpaceship(int laserShotTime, int missileShotTime) {

		reactLaserShoot = TimeAction(laserShotTime);
		reactMissileShoot = TimeAction(missileShotTime);
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		if (!entity->hasComponent<SpriteComponent>()) {

			entity->addComponent<SpriteComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {

		if (reactShootAnimation.check() or reactDamageAnimation.check()) sprite->playAnimation("base");

		if (energy <= 0) entity->destroy();

		checkForInputs();
		correctPositionInTheArea();
		
		// Setting wall entityes
		Collision::subtractCollisionMTV(&entity->getComponent<ColliderComponent>(), satId);

		// After checking collisions with walls reset the position of the sprite.
		sprite->resetPosition();
	}

	void shot() {
		
		if      (weapons[SelectedWeapon] == "laser")   shotLaser();
		else if (weapons[SelectedWeapon] == "missile") shotMissile();
	}

	void checkForInputs() {

		const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

		if (KeyboardState[SDL_SCANCODE_X]) reactDamage();

		if (KeyboardState[SDL_SCANCODE_M]) {

			if (alreadySwitched) {
				switchWeapon();
				alreadySwitched = false;
			}
		}

		else alreadySwitched = true;

		if (KeyboardState[SDL_SCANCODE_SPACE]) {

			if (automaticWeapon) {
				shot();
				automaticWeapon = false;
			}
		}

		else automaticWeapon = true;
	}

	void shotLaser() {

		if (reactLaserShoot.check() and !reactDamageAnimation.isActive()) {

			// definning the position where spawn the laser
			int xposition = position->scale * (position->position.x + position->width - laserPositionCorrection.x);
			int yposition = position->scale * (position->position.y + position->height - laserPositionCorrection.y);

			reactLaserShoot.init();

			SDL_Rect laserSpace = { xposition, yposition, 8, 2 };

			auto& bullet = entity->manager.addEntity();

			bullet.addComponent<PositionComponent>(laserSpace, 1);
			bullet.addComponent<SpriteComponent>("sprites//lasers//laser.png");
			bullet.addComponent<ColliderComponent>(laserId);
			bullet.addComponent<BulletComponent>();

			bullet.getComponent<PositionComponent>().setSpeed(10, 0);

			bullet.addGroup(groupBullets);

			sprite->playAnimation("shot");

			reactShootAnimation.init();
		}
	}

	void shotMissile() {

		if (reactMissileShoot.check() and !reactDamageAnimation.isActive() and missiles > 0) {

			// defining the position where spawn the missile
			int xposition = position->scale * (position->position.x + position->width - missilePositionCorrection.x);
			int yposition = position->scale * (position->position.y + position->height - missilePositionCorrection.y);
			int xspeed = position->getActualMovement().x + 8;
			int yspeed = position->getActualMovement().y;

			Explosion explosion = { "sprites//explosions//explosion.png", 40, 60 };
			SDL_Rect missileSpace = { xposition, yposition, 12, 5 };

			reactMissileShoot.init();

			auto& bullet = entity->manager.addEntity();

			bullet.addComponent<PositionComponent>(missileSpace, 1);
			bullet.addComponent<SpriteComponent>("sprites//missiles//missile.png", true);
			bullet.addComponent<ColliderComponent>(missileId);
			bullet.addComponent<BulletComponent>();
			bullet.addComponent<MissileComponent>();
			bullet.addComponent<ExplodeComponent>(explosion, 50, 50);

			bullet.getComponent<PositionComponent>().setSpeed(xspeed, yspeed);
			bullet.getComponent<SpriteComponent>().addAnimation("base", 4, 0, 10);
			bullet.getComponent<SpriteComponent>().playAnimation("base");

			bullet.addGroup(groupBullets);

			sprite->playAnimation("shot");

			reactShootAnimation.init();

			missiles -= 1;
		}
	}

	void correctPositionInTheArea() {

		// Move the player inside the render area if it is outside

		statusPosition status = position->isCompletelyOnRender();

		position->position.x -= status.xdistance;
		position->position.y -= status.ydistance;
	}

	void switchWeapon() {

		if (SelectedWeapon < (sizeof(weapons) / sizeof(std::string)) - 1) SelectedWeapon++;
		else SelectedWeapon = 0;
	}

	std::string getSelectedWeapon() {

		return weapons[SelectedWeapon];
	}

	void reactDamage() {

		// The reaction to do when the player is hitted by a bullet

		reactDamageAnimation = true;
		sprite->playAnimation("damage");
		reactDamageAnimation.init();
	}
};