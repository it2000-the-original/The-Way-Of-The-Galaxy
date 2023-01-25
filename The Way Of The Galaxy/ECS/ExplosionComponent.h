#pragma once
#include "Components.h"
#include "TimeAction.h"

class ExplosionComponent : public Component {

private:

	TimeAction explosionDuration = TimeAction(100);

public:

	ExplosionComponent(int mExplosionDuration) {

		explosionDuration.setDuration(mExplosionDuration);
	}

	void init() override {
	
		explosionDuration.init();
	}

	void update() override {

		if (explosionDuration.check()) {

			entity->destroy();
		}
	}
};
