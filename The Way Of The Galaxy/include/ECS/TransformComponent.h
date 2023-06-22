#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component {

private:

	PositionComponent* position;

	Vector2D velocity = Vector2D(1, 1);
	Vector2D acceleration = Vector2D(1, 1);

	// Variable to comfirm that precedently a
	// function to move was called

	bool rxmoved = false;
	bool lxmoved = false;
	bool uymoved = false;
	bool dymoved = false;

public:

	TransformComponent() {}

	TransformComponent(Vector2D mVelocity, Vector2D mAcceleration) {

		velocity = mVelocity;
		acceleration = mAcceleration;
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
	}

	void update() override {

		// Delete all movements if two functions opposite are called
		if (rxmoved and lxmoved) rxmoved = lxmoved = false;
		if (dymoved and uymoved) dymoved = uymoved = false;

		// Decellerate the mote if isn't pressed a key to maintain it
		if (position->velocity.x > 0 and !rxmoved) position->velocity.x -= acceleration.x; else rxmoved = false;
		if (position->velocity.x < 0 and !lxmoved) position->velocity.x += acceleration.x; else lxmoved = false;
		if (position->velocity.y > 0 and !dymoved) position->velocity.y -= acceleration.y; else dymoved = false;
		if (position->velocity.y < 0 and !uymoved) position->velocity.y += acceleration.y; else uymoved = false;
	}

	void setAcceleration(Vector2D mAcceleration) {

		acceleration = mAcceleration;
	}

	void setVelocity(Vector2D mVelocity) {

		velocity = mVelocity;
	}

	void moveUp() {

		if (position->velocity.y > -velocity.y) {

			position->velocity.y -= acceleration.y;
		}

		uymoved = true;
	}

	void moveDown() {

		if (position->velocity.y < velocity.y) {

			position->velocity.y += acceleration.y;
		}

		dymoved = true;
	}

	void moveLeft() {

		if (position->velocity.x > -velocity.x) {

			position->velocity.x -= acceleration.x;
		}

		lxmoved = true;
	}

	void moveRight() {

		if (position->velocity.x < velocity.x) {

			position->velocity.x += acceleration.x;
		}

		rxmoved = true;
	}
};
