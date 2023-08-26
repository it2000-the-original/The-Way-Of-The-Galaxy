#include "ECS/custom/PlayerSpaceship.h"
#include "AssetsManager.h"

PlayerSpaceship::PlayerSpaceship() {

	reactLaserShoot = TimeAction(100);
	reactMissileShoot = TimeAction(300);
}

PlayerSpaceship::PlayerSpaceship(int laserShotTime, int missileShotTime) {

	reactLaserShoot = TimeAction(laserShotTime);
	reactMissileShoot = TimeAction(missileShotTime);
}

void PlayerSpaceship::init() {

	if (!entity->hasComponent<PositionComponent>()) {

		entity->addComponent<PositionComponent>();
	}

	if (!entity->hasComponent<TransformComponent>()) {

		entity->addComponent<TransformComponent>();
	}

	if (!entity->hasComponent<SpriteComponent>()) {

		entity->addComponent<SpriteComponent>();
	}

	position = &entity->getComponent<PositionComponent>();
	transform = &entity->getComponent<TransformComponent>();
	sprite = &entity->getComponent<SpriteComponent>();

	sprite->playAnimation("base");

	transform->setAcceleration(Vector2D(0.125, 0.125));
	transform->setVelocity(Vector2D(5, 5));
}

void PlayerSpaceship::update() {

	if (reactShootAnimation.check() or reactDamageAnimation.check()) sprite->playAnimation("base");

	if (energy <= 0) entity->destroy();

	checkForInputs();
	correctPositionInTheArea();

}

void PlayerSpaceship::onCollision2D(Collision2D collision) {

	if (collision.colliderB == satId) {

		collision.penetration.Round(3);

		position->velocity -= collision.penetration;
		position->position -= collision.penetration;

		transform->wall = true;
	}
}

void PlayerSpaceship::shot() {

	if (weapons[SelectedWeapon] == "laser") shotLaser();
	else if (weapons[SelectedWeapon] == "missile") shotMissile();
}

void PlayerSpaceship::shotLaser() {

	if (reactLaserShoot.check() and !reactDamageAnimation.isActive()) {

		// definning the position where spawn the laser
		int xposition = int(position->scale * (position->position.x + position->width - laserPositionCorrection.x));
		int yposition = int(position->scale * (position->position.y + position->height - laserPositionCorrection.y));

		auto& laser = Engine::assets.loadAsset("laser", xposition, yposition);

		laser.getComponent<PositionComponent>().setSpeed(10, 0);

		reactShootAnimation.init();
		reactLaserShoot.init();

		sprite->playAnimation("shot");
	}
}

void PlayerSpaceship::shotMissile() {

	if (reactMissileShoot.check() and !reactDamageAnimation.isActive() and missiles > 0) {

		// defining the position where spawn the missile
		int xposition = int(position->scale * (position->position.x + position->width - missilePositionCorrection.x));
		int yposition = int(position->scale * (position->position.y + position->height - missilePositionCorrection.y));

		auto& missile = Engine::assets.loadAsset("missile", xposition, yposition);

		missile.getComponent<PositionComponent>().setSpeed(int(position->velocity.x), int(position->velocity.y));
		missile.getComponent<MissileComponent>().setAcceleration(0.5);

		reactMissileShoot.init();
		reactShootAnimation.init();

		sprite->playAnimation("shot");

		missiles -= 1;
	}
}

void PlayerSpaceship::checkForInputs() {

	const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

	// moving the entity if one of the interested keys is pressed.
	if (KeyboardState[SDL_SCANCODE_D] or KeyboardState[SDL_SCANCODE_RIGHT]) transform->moveRight();
	if (KeyboardState[SDL_SCANCODE_A] or KeyboardState[SDL_SCANCODE_LEFT])  transform->moveLeft();
	if (KeyboardState[SDL_SCANCODE_W] or KeyboardState[SDL_SCANCODE_UP]) 	transform->moveUp();
	if (KeyboardState[SDL_SCANCODE_S] or KeyboardState[SDL_SCANCODE_DOWN])  transform->moveDown();
	if (KeyboardState[SDL_SCANCODE_E]) position->angle += 1;
	if (KeyboardState[SDL_SCANCODE_Q]) position->angle -= 1;

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

void PlayerSpaceship::correctPositionInTheArea() {

	// Move the player inside the render area if it is outside

	statusPosition status = position->isCompletelyOnRender();

	position->position.x -= round(status.xdistance * pow(10, 3)) / pow(10, 3);
	position->position.y -= round(status.ydistance * pow(10, 3)) / pow(10, 3);
	position->velocity.x -= round(status.xdistance * pow(10, 3)) / pow(10, 3);
	position->velocity.y -= round(status.ydistance * pow(10, 3)) / pow(10, 3);
}

void PlayerSpaceship::switchWeapon() {

	if (SelectedWeapon < (sizeof(weapons) / sizeof(std::string)) - 1) SelectedWeapon++;
	else SelectedWeapon = 0;
}

std::string PlayerSpaceship::getSelectedWeapon() {

	return weapons[SelectedWeapon];
}

void PlayerSpaceship::reactDamage() {

	// The reaction to do when the player is hitted by a bullet

	reactDamageAnimation = true;
	sprite->playAnimation("damage");
	reactDamageAnimation.init();
}