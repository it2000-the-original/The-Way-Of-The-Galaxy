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

	linearDecelleration();

	externalMotion = Vector2D(0, 0);

	rxmoved = lxmoved = dymoved = uymoved = false;

	wall = false;
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

		position->velocity.y = position->velocity.y * velocity.x / position->velocity.x;
		position->velocity.x = velocity.x;
	}

	else if (position->velocity.x < -velocity.x) {

		position->velocity.y = position->velocity.y * -velocity.x / position->velocity.x;
		position->velocity.x = -velocity.x;
	}

	if (position->velocity.y > velocity.y) {

		position->velocity.x = position->velocity.x * velocity.y / position->velocity.y;
		position->velocity.y = velocity.y;
	}

	else if (position->velocity.y < -velocity.y) {

		position->velocity.x = position->velocity.x * -velocity.y / position->velocity.y;
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

void TransformComponent::noControlledXAxisDecelleration() {

	if (position->velocity.x > 0) {

		if (position->velocity.x < acceleration.x) {

			position->velocity.x = 0;
		}

		else position->velocity.x -= acceleration.x;
	}

	else if (position->velocity.x < 0) {

		if (position->velocity.x > -acceleration.x) {

			position->velocity.x = 0;
		}

		else position->velocity.x += acceleration.x;
	}
}

void TransformComponent::noControlledYAxisDecelleration() {

	if (position->velocity.y > 0) {

		if (position->velocity.y < acceleration.y) {

			position->velocity.y = 0;
		}

		else position->velocity.y -= acceleration.y;
	}

	else if (position->velocity.y < 0) {

		if (position->velocity.y > -acceleration.y) {

			position->velocity.y = 0;
		}

		else position->velocity.y += acceleration.y;
	}
}

void TransformComponent::linearDecelleration() {

	// Decelerate the entity gradualy and linearly

	if (!rxmoved and !lxmoved and !uymoved and !dymoved) {

		double xFractionalDecelleration = fabs(position->velocity.x) / acceleration.x;
		double yFractionalDecelleration = fabs(position->velocity.y) / acceleration.y;

		if (xFractionalDecelleration > yFractionalDecelleration) {

			noControlledXAxisDecelleration();

			if (position->velocity.x == 0) {

				position->velocity.y = 0;
			}

			else {

				if (position->velocity.y > 0) {

					position->velocity.y -= position->velocity.y / xFractionalDecelleration;
				}

				else if (position->velocity.y) {

					position->velocity.y -= position->velocity.y / xFractionalDecelleration;
				}
			}
		}

		else {

			noControlledYAxisDecelleration();

			if (position->velocity.y == 0) {

				position->velocity.x = 0;
			}

			else {

				if (position->velocity.x > 0) {

					position->velocity.x -= position->velocity.x / yFractionalDecelleration;
				}

				else if (position->velocity.x) {

					position->velocity.x -= position->velocity.x / yFractionalDecelleration;
				}
			}
		}
		
		position->velocity.Round(3);
	}

	else {
		
		xAxisDecelleration();
		yAxisDecelleration();
	}
}