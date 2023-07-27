#include "ECS/Components.h"

ExplosionComponent::ExplosionComponent(int mExplosionDuration) {

	explosionDuration.setDuration(mExplosionDuration);
}

void ExplosionComponent::init() {

	explosionDuration.init();
}

void ExplosionComponent::update() {

	if (explosionDuration.check()) {

		entity->destroy();
	}
}