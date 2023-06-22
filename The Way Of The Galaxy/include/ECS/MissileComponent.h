#pragma once
#include "PositionComponent.h"
#include "TimeAction.h"

class MissileComponent : public Component {
	
private:

	PositionComponent* position;
	SpriteComponent* sprite;
	Vector2D acceleration = Vector2D(0.25, 0);

public:

	int damage;

	MissileComponent() {
		
		damage = 5;
	}

	MissileComponent(int mDamage) {
		
		damage = mDamage;
	}

	MissileComponent(int mDamage, float mAcceleration) {

		damage = mDamage;
		acceleration = Vector2D(mAcceleration, 0);
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

		sprite->playAnimation("base");
	}

	void update() override {

		position->velocity += acceleration;
	}

	void setAcceleration(float mAcceleration) {

		acceleration = Vector2D(mAcceleration, 0);
	}
};
