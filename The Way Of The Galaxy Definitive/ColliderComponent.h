#pragma once
#include "SDL.h"
#include "Components.h"
#include <string>

class ColliderComponent : public Component {

public:

	SDL_Rect collider;
	std::size_t id;

	PositionComponent* position;

	ColliderComponent(std::size_t t) {

		id = t;
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
		Engine::colliders.push_back(this);
	}

	void update() override {

		collider.x = static_cast<int>(position->position.x);
		collider.y = static_cast<int>(position->position.y);
		collider.w = position->width * position->scale;
		collider.h = position->height * position->scale;
	}
};