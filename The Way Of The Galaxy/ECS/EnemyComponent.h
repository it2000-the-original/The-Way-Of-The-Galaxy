#pragma once
#include "Components.h"

class EnemyComponent : public Component {

private:

	PositionComponent* position;

public:

	EnemyComponent() {}

	void init() override {
		
		if (entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
	}

	void update() override {
	
		if (!position->isOnRender().xy) {

			entity->destroy();
		}
	}	
};
