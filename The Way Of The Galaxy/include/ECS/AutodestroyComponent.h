#pragma once
#include "PositionComponent.h"

#include "Engine.h"

class AutodestroyComponent : public Component {

private:

	PositionComponent* position;

public:

	AutodestroyComponent() {}

	~AutodestroyComponent() {
		
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