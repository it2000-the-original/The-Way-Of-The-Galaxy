#pragma once
#include "ECS.h"
#include "TimeAction.h"

class ExplosionComponent : public Component {

private:

	TimeAction explosionDuration;

public:

	ExplosionComponent(int mExplosionDuration);

	void init() override;
	void update() override;
};
