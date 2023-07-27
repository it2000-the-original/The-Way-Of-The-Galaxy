#include "ECS/Components.h"
#include "Vector2D.h"

TransformComponent::TransformComponent() {}

TransformComponent::TransformComponent(Vector2D mVelocity, Vector2D mAcceleration) {

	velocity = mVelocity;
	acceleration = mAcceleration;
}

void TransformComponent::init() {

	if (!entity->hasComponent<PositionComponent>()) {

		entity->addComponent<PositionComponent>();
	}

	position = &entity->getComponent<PositionComponent>();
}

void TransformComponent::update() {

	// Delete all movements if two functions opposite are called

	if (rxmoved and lxmoved) rxmoved = lxmoved = false;
	if (dymoved and uymoved) dymoved = uymoved = false;

	// Decellerate the mote if isn't pressed a key to maintain it

	if (!rxmoved and !lxmoved and !dymoved and !uymoved) {

		linearDecelleration();
	}

	else {

		bool nox = false; // decellerate x axis
		bool noy = false; // decellerate y axis

		if (externalMotion != Vector2D(0, 0)) {

			if (rxmoved or lxmoved) {

				if (position->velocity.y * externalMotion.y > 0) noy = true;
			}

			else {

				if (position->velocity.x * externalMotion.x > 0) nox = true;
			}
		}

		if (!nox) xAxisDecelleration();
		if (!noy) yAxisDecelleration();

		removeVelocityExcesses();
	}

	externalMotion = Vector2D(0, 0);

	rxmoved = lxmoved = dymoved = uymoved = false;
}

void TransformComponent::setAcceleration(Vector2D mAcceleration) {

	acceleration = mAcceleration;
}

void TransformComponent::setVelocity(Vector2D mVelocity) {

	velocity = mVelocity;
}

void TransformComponent::moveUp() {

	if (position->velocity.y > -velocity.y) {

		position->velocity.y -= acceleration.y;
	}

	uymoved = true;
}

void TransformComponent::moveDown() {

	if (position->velocity.y < velocity.y) {

		position->velocity.y += acceleration.y;
	}

	dymoved = true;
}

void TransformComponent::moveLeft() {

	if (position->velocity.x > -velocity.x) {

		position->velocity.x -= acceleration.x;
	}

	lxmoved = true;
}

void TransformComponent::moveRight() {

	if (position->velocity.x < velocity.x) {

		position->velocity.x += acceleration.x;
	}

	rxmoved = true;
}

void TransformComponent::setExternalMotion(Vector2D motion) {

	externalMotion = motion;
}

void TransformComponent::removeVelocityExcesses() {

	// Removing excesses from velocity

	if (position->velocity.x > velocity.x) {

		position->velocity.x = velocity.x;
	}

	else if (position->velocity.x < -velocity.x) {

		position->velocity.x = -velocity.x;
	}

	if (position->velocity.y > velocity.y) {

		position->velocity.y = velocity.y;
	}

	else if (position->velocity.y < -velocity.y) {

		position->velocity.y = -velocity.y;
	}
}

void TransformComponent::xAxisDecelleration() {

	if (position->velocity.x > 0 and !rxmoved) {

		if (position->velocity.x < acceleration.x) {

			position->velocity.x = 0;
		}

		else position->velocity.x -= acceleration.x;
	}

	else if (position->velocity.x < 0 and !lxmoved) {

		if (position->velocity.x > -acceleration.x) {

			position->velocity.x = 0;
		}

		else position->velocity.x += acceleration.x;
	}
}

void TransformComponent::yAxisDecelleration() {

	if (position->velocity.y > 0 and !dymoved) {

		if (position->velocity.y < acceleration.y) {

			position->velocity.y = 0;
		}

		else position->velocity.y -= acceleration.y;
	}

	else if (position->velocity.y < 0 and !uymoved) {

		if (position->velocity.y > -acceleration.y) {

			position->velocity.y = 0;
		}

		else position->velocity.y += acceleration.y;
	}
}

void TransformComponent::linearDecelleration() {

	// Calculating the direction of the velocity and the module of it, and of the acceleration

	float velocityAngle = atan2(position->velocity.y, position->velocity.x);
	float accelerationModule = float(sqrt(pow(acceleration.x, 2) + pow(acceleration.y, 2)));
	float velocityModule = float(sqrt(pow(position->velocity.x, 2) + pow(position->velocity.y, 2)));

	if (velocityModule < accelerationModule) {

		position->velocity.Zero();
	}

	else {

		velocityModule -= accelerationModule;

		position->velocity = Vector2D(
			velocityModule * cos(velocityAngle),
			velocityModule * sin(velocityAngle)
		).Round(3);
	}
}