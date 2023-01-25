#pragma once
#include "Components.h"
#include "Engine.h"

class BulletComponent : public Component {

private:

	PositionComponent* position;

public:

	BulletComponent() {}

	~BulletComponent() {
		
		position = nullptr;
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
	}
	
	void update() override {

		// Chek if the posion is in the render area.
		if (!position->isOnRender().xy) entity->destroy();
	}
};