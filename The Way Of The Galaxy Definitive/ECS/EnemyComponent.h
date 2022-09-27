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
};
