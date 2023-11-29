#include "ECS/Components.h"

AutodestroyComponent::AutodestroyComponent() : control(xy) {}

AutodestroyComponent::AutodestroyComponent(std::size_t _control) : control(_control) {}

void AutodestroyComponent::init() {

	if (!entity->hasComponent<PositionComponent>()) {

		entity->addComponent<PositionComponent>();
	}

	position = &entity->getComponent<PositionComponent>();
}

void AutodestroyComponent::update() {

	// Chek if the posion is in the render area.

	switch (control) {

	case lx:

		if (!position->isOnRender().xl) entity->destroy();
		break;

	case rx:

		if (!position->isOnRender().xr) entity->destroy();
		break;

	case x:

		if (!position->isOnRender().x) entity->destroy();
		break;

	case uy:

		if (!position->isOnRender().yu) entity->destroy();
		break;

	case dy:

		if (!position->isOnRender().yd) entity->destroy();
		break;

	case y:

		if (!position->isOnRender().y) entity->destroy();
		break;

	case xy:

		if (!position->isOnRender().xy) entity->destroy();
		break;
	}
}

AutodestroyComponent::~AutodestroyComponent() {

	position = nullptr;
}