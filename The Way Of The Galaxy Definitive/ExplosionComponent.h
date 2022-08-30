#pragma once
#include "Components.h"

class ExplosionComponent : public Component {

private:

	Uint32 initialTime;
	int explosionDuration;

public:

	ExplosionComponent(int mExplosionDuration) {

		explosionDuration = mExplosionDuration;
	}

	void init() override {
	
		initialTime = SDL_GetTicks();
	}

	void update() override {

		if (SDL_GetTicks() - initialTime > explosionDuration) {

			entity->destroy();
		}
	}
};
