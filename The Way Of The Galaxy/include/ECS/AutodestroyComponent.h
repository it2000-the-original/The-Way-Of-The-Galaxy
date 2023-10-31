#pragma once
#include "ECS.h"

enum controls : std::size_t {

	lx, rx, x,
	uy, dy, y,
	xy
};

class PositionComponent;

class AutodestroyComponent : public Component {

private:

	PositionComponent* position;
	std::size_t control;

public:

	AutodestroyComponent();
	AutodestroyComponent(std::size_t _control);

	void init() override;
	void update() override;

	~AutodestroyComponent();
};