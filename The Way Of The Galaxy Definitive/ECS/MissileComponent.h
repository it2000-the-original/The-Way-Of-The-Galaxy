#pragma once
#include "Components.h"
#include "TimeAction.h"

class MissileComponent : public Component {
	
private:

	PositionComponent* position;
	TimeAction missileAcceleration;

public:

	int damage;

	MissileComponent() {
		
		damage = 5;
		missileAcceleration = TimeAction(35);
	}

	MissileComponent(int mDamage) {
		
		damage = mDamage;
	}

	void init() override {

		position = &entity->getComponent<PositionComponent>();
		missileAcceleration.init();
	}

	void update() override {

		if (missileAcceleration.check()) {

			position->velocity.x += 1;
			missileAcceleration.init();
		}
	}
};
