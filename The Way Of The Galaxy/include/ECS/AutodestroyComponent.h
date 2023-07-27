#pragma once
#include "ECS.h"

class PositionComponent;

class AutodestroyComponent : public Component {

private:

	PositionComponent* position;

public:

	AutodestroyComponent();

	void init() override;
	void update() override;

	~AutodestroyComponent();
};