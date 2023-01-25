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
		missileAcceleration = TimeAction(35);
	}

	MissileComponent(int mDamage, int mAcceleration) {

		damage = mDamage;
		missileAcceleration = TimeAction(mAcceleration);
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

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
