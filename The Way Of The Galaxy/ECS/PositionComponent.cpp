#include "ECS/Components.h"
#include "Vector2D.h"
#include "Engine.h"

PositionComponent::PositionComponent() {}

PositionComponent::PositionComponent(int x, int y) {

	position.x = float(x);
	position.y = float(y);
}

PositionComponent::PositionComponent(SDL_Rect space, int s) {

	position.x = float(space.x);
	position.y = float(space.y);

	width = space.w;
	height = space.h;
	scale = s;
}

PositionComponent::PositionComponent(SDL_Rect space, int s, int a) {

	position.x = float(space.x);
	position.y = float(space.y);

	width = space.w;
	height = space.h;
	scale = s;
	angle = a;
}

void PositionComponent::init() {

	velocity.Zero();
}

void PositionComponent::update() {

	position.Round(3);
	velocity.Round(3);

	previousPosition = position;
	angle += rotationSpeed;
	position += velocity;
}

void PositionComponent::setPosition(int x, int y) {

	position.x = float(x);
	position.y = float(y);
}

void PositionComponent::setSpeed(int x, int y) {

	velocity.x = float(x);
	velocity.y = float(y);
}

void PositionComponent::setRotationSpeed(int mRotationSpeed) {

	rotationSpeed = mRotationSpeed;
}

statusPosition PositionComponent::isOnRender() {

	// This function return a status struct to know if the entity is in
	// the render area and to get distance to the margin in x and y

	statusPosition status;
	SDL_Rect rectangle = getVisualRectangle();

	if (rectangle.x >= -rectangle.w)                status.xl = true;
	if (rectangle.x <= renderwidth)                 status.xr = true;
	if (rectangle.y >= -rectangle.h + statusheight) status.yu = true;
	if (rectangle.y <= renderheight)                status.yd = true;

	if (status.xl == true and status.xr == true) status.x = true;
	if (status.yu == true and status.yd == true) status.y = true;

	if (status.x == true and status.y == true) status.xy = true;

	if      (!status.x and status.xl) status.xdistance = rectangle.x - renderwidth;
	else if (!status.x and status.xr) status.xdistance = rectangle.x + rectangle.w;
	if      (!status.y and status.yu) status.ydistance = rectangle.y - renderheight;
	else if (!status.y and status.yd) status.ydistance = rectangle.y + rectangle.h - statusheight;

	return status;
}

statusPosition PositionComponent::isCompletelyOnRender() {

	// This function return a status struct to know if the entity 
	// is completely with all the space of the sprite in
	// the render area and to get distance to the margin in x and y

	statusPosition status;
	SDL_Rect rectangle = getVisualRectangle();

	if (rectangle.x >= 0)                          status.xl = true;
	if (rectangle.x <= renderwidth - rectangle.w)  status.xr = true;
	if (rectangle.y >= statusheight)               status.yu = true;
	if (rectangle.y <= renderheight - rectangle.h) status.yd = true;

	if (status.xl == true and status.xr == true) status.x = true;
	if (status.yu == true and status.yd == true) status.y = true;

	if (status.x == true and status.y == true) status.xy = true;

	if      (!status.x and status.xl) status.xdistance = rectangle.x + rectangle.w - renderwidth;
	else if (!status.x and status.xr) status.xdistance = rectangle.x;
	if      (!status.y and status.yu) status.ydistance = rectangle.y + rectangle.h - renderheight;
	else if (!status.y and status.yd) status.ydistance = rectangle.y - statusheight;

	return status;
}

Point PositionComponent::getCenterPoint() {

	return Point(
		position.x + width * scale / 2,
		position.y + height * scale / 2
	);
}

Vector2D PositionComponent::getActualMovement() {

	return position - previousPosition;
}

SDL_Rect PositionComponent::getVisualRectangle() {

	if (angle != 0) {

		// Calculate width and height of an inclinated rectangle

		SDL_Rect rectangle;

		float firstDiagonalAngle = atan2(height, width);
		float secondDiagonalAngle = -firstDiagonalAngle;
		float diagonalSize = sqrt(pow(width * scale, 2) + pow(height * scale, 2));

		firstDiagonalAngle += angle * 3.1415 / 180;
		secondDiagonalAngle += angle * 3.1415 / 180;

		if (fabs(diagonalSize * cos(firstDiagonalAngle)) > 
			fabs(diagonalSize * cos(secondDiagonalAngle))) {

			rectangle.w = fabs(diagonalSize * cos(firstDiagonalAngle));
		}

		else {

			rectangle.w = fabs(diagonalSize * cos(secondDiagonalAngle));
		}

		if (fabs(diagonalSize * sin(firstDiagonalAngle)) > 
			fabs(diagonalSize * sin(secondDiagonalAngle))) {

			rectangle.h = fabs(diagonalSize * sin(firstDiagonalAngle));
		}

		else {

			rectangle.h = fabs(diagonalSize * sin(secondDiagonalAngle));
		}

		if (rectangle.w > width * scale) {

			rectangle.x = position.x - (rectangle.w - width * scale) / 2;
		}

		else {

			rectangle.x = position.x + (width * scale - rectangle.w) / 2;
		}

		if (rectangle.h > height * scale) {

			rectangle.y = position.y - (rectangle.h - height * scale) / 2;
		}

		else {

			rectangle.y = position.y + (height * scale - rectangle.h) / 2;
		}

		std::cout << rectangle.w << " " << rectangle.h << std::endl;

		return rectangle;
	}

	return {position.x, position.y, width * scale, height * scale};
}