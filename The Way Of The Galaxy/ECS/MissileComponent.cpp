#include "ECS/Components.h"
#include "Vector2D.h"

MissileComponent::MissileComponent() {

	damage = 5;
}

MissileComponent::MissileComponent(int mDamage) {

	damage = mDamage;
}

MissileComponent::MissileComponent(int mDamage, float mAcceleration) {

	damage = mDamage;
	acceleration = Vector2D(mAcceleration, 0);
}

void MissileComponent::init() {

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

void MissileComponent::update() {

	position->velocity += acceleration;
}

void MissileComponent::setAcceleration(float mAcceleration) {

	acceleration = Vector2D(mAcceleration, 0);
}