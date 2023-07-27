#include "ECS/Components.h"

AutodestroyComponent::AutodestroyComponent() {}

void AutodestroyComponent::init() {

	if (!entity->hasComponent<PositionComponent>()) {

		entity->addComponent<PositionComponent>();
	}

	position = &entity->getComponent<PositionComponent>();
}

void AutodestroyComponent::update() {

	// Chek if the posion is in the render area.
	if (!position->isOnRender().xy) entity->destroy();
}

AutodestroyComponent::~AutodestroyComponent() {

	position = nullptr;
}