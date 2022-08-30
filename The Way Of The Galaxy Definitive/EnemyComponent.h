#pragma once
#include "Components.h"
#include "GroupLabels.h"

class EnemyComponent : public Component {

private:

	PositionComponent* position;

public:

	EnemyComponent() {}

	void init() override {
		
		position = &entity->getComponent<PositionComponent>();
	}

	void update() override {
	
		if (!position->isOnRender()) {

			entity->destroy();
		}
	}

	void explode() {

		auto& explosion = entity->manager.addEntity();
		explosion.addComponent<PositionComponent>(position->position.x + position->width / 2 - 50, position->position.y + position->height / 2 - 50, 50, 50, 2);
		explosion.addComponent<SpriteComponent>("sprites\\explosions\\explosion.png", true);
		explosion.getComponent<SpriteComponent>().addAnimation("explosion", 40, 0, 60);
		explosion.getComponent<SpriteComponent>().playAnimation("explosion");
		explosion.addComponent<ExplosionComponent>(60 * 40);
		explosion.addGroup(groupExplosions);
		entity->destroy();
	}
};
