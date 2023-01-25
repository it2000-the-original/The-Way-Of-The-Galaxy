#pragma once
#include "Components.h"

class KeyboardController : public Component {

private:

	PositionComponent* position;

public:

	KeyboardController() {}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
	}

	void update() override {

		// get the state of every key
		const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

		// moving the entity if one of the interested keys is pressed.
		if (KeyboardState[SDL_SCANCODE_D] or KeyboardState[SDL_SCANCODE_RIGHT]) position->moveRight();
		if (KeyboardState[SDL_SCANCODE_A] or KeyboardState[SDL_SCANCODE_LEFT])  position->moveLeft();
		if (KeyboardState[SDL_SCANCODE_W] or KeyboardState[SDL_SCANCODE_UP]) 	position->moveUp();
		if (KeyboardState[SDL_SCANCODE_S] or KeyboardState[SDL_SCANCODE_DOWN])  position->moveDown();
	}
};