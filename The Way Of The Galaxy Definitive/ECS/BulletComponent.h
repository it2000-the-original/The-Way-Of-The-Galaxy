#pragma once
#include "Components.h"
#include "Engine.h"

class BulletComponent : public Component {

private:

	PositionComponent* position;

public:

	BulletComponent() {}

	~BulletComponent() {
		
		position = nullptr;
	}

	void init() override {

		position = &entity->getComponent<PositionComponent>();
	}
	
	void update() override {

		if (!position->isOnRender())
			entity->destroy();
	}
};