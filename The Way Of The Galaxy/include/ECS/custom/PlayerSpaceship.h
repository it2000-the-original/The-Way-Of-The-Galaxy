#pragma once
#include "ECS/Components.h"
#include "TimeAction.h"
#include "Engine.h"
#include <string>

class PlayerSpaceship : public Component {

private:

	PositionComponent* position;
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;

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
	
	PlayerSpaceship();
	PlayerSpaceship(int laserShotTime, int missileShotTime);

	void init() override;
	void update() override;
	void onCollision2D(Collision2D collision) override;

	void shot();
	void shotLaser();
	void shotMissile();
	void checkForInputs();
	void correctPositionInTheArea();
	void switchWeapon();
	std::string getSelectedWeapon();
	void reactDamage();
};