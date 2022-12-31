#pragma once
#include "Components.h"

class PlayerComponent;

class KeyboardController : public Component {

private:

	PositionComponent* position;
	const Uint8* KeyboardState;
	bool automaticWeapon = true;
	bool alreadySwitched = true;

public:

	KeyboardController() {}

	~KeyboardController() {

		position = nullptr;
		delete KeyboardState;
		KeyboardState = nullptr;
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
	}

	void update() override {

		KeyboardState = SDL_GetKeyboardState(NULL);

		if (KeyboardState[SDL_SCANCODE_D] or KeyboardState[SDL_SCANCODE_RIGHT]) position->moveRight();
		if (KeyboardState[SDL_SCANCODE_A] or KeyboardState[SDL_SCANCODE_LEFT])  position->moveLeft();
		if (KeyboardState[SDL_SCANCODE_W] or KeyboardState[SDL_SCANCODE_UP]) 	position->moveUp();
		if (KeyboardState[SDL_SCANCODE_S] or KeyboardState[SDL_SCANCODE_DOWN])  position->moveDown();
	}
};