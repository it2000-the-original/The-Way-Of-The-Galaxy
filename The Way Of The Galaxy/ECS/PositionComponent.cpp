#include "ECS/Components.h"
#include "Vector2D.h"
#include "Engine.h"

PositionComponent::PositionComponent() {}

PositionComponent::PositionComponent(int x, int y) {

	position.x = float(x);
	position.y = float(y);
}

PositionComponent::PositionComponent(Rectangle space, int s) {

	position.x = space.x;
	position.y = space.y;

	width = int(space.w);
	height = int(space.h);
	scale = s;
}

PositionComponent::PositionComponent(Rectangle space, int s, int a) {

	position.x = space.x;
	position.y = space.y;

	width = int(space.w);
	height = int(space.h);
	scale = s;
	angle = a;
}

void PositionComponent::init() {

	velocity.Zero();
}

void PositionComponent::update() {

	angle += rotationSpeed;
	position += velocity;
}

void PositionComponent::setPosition(double x, double y) {

	position.x = x;
	position.y = y;
}

void PositionComponent::setSpeed(double x, double y) {

	velocity.x = x;
	velocity.y = y;
}

void PositionComponent::setRotationSpeed(int mRotationSpeed) {

	rotationSpeed = mRotationSpeed;
}

statusPosition PositionComponent::isOnRender() {

	// This function return a status struct to know if the entity is in
	// the render area and to get distance to the margin in x and y

	statusPosition status;
	Rectangle rectangle = getVisualRectangle();

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
	Rectangle rectangle = getVisualRectangle();

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

Rectangle PositionComponent::getVisualRectangle() {

	if (angle != 0) {

		// Calculate width and height of an inclinated rectangle

		Rectangle rectangle;

		float firstDiagonalAngle = float(atan2(height, width));
		float secondDiagonalAngle = -firstDiagonalAngle;
		float diagonalSize = float(sqrt(pow(width * scale, 2) + pow(height * scale, 2)));

		firstDiagonalAngle += float(angle * 3.1415 / 180);
		secondDiagonalAngle += float(angle * 3.1415 / 180);

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

		return rectangle;
	}

	return {position.x, position.y, double(width * scale), double(height * scale)};
}