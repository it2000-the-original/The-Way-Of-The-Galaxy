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

	if (position.x >= -width * scale)                 status.xl = true;
	if (position.x <= renderwidth)                    status.xr = true;
	if (position.y >= -height * scale + statusheight) status.yu = true;
	if (position.y <= renderheight)                   status.yd = true;

	if (status.xl == true and status.xr == true) status.x = true;
	if (status.yu == true and status.yd == true) status.y = true;

	if (status.x == true and status.y == true) status.xy = true;

	if      (!status.x and status.xl) status.xdistance = position.x - renderwidth;
	else if (!status.x and status.xr) status.xdistance = position.x + width * scale;
	if      (!status.y and status.yu) status.ydistance = position.y - renderheight;
	else if (!status.y and status.yd) status.ydistance = position.y + height * scale - statusheight;

	return status;
}

statusPosition PositionComponent::isCompletelyOnRender() {

	// This function return a status struct to know if the entity 
	// is completely with all the space of the sprite in
	// the render area and to get distance to the margin in x and y

	statusPosition status;

	if (position.x >= 0)                             status.xl = true;
	if (position.x <= renderwidth - width * scale)   status.xr = true;
	if (position.y >= statusheight)                  status.yu = true;
	if (position.y <= renderheight - height * scale) status.yd = true;

	if (status.xl == true and status.xr == true) status.x = true;
	if (status.yu == true and status.yd == true) status.y = true;

	if (status.x == true and status.y == true) status.xy = true;

	if      (!status.x and status.xl) status.xdistance = position.x + width * scale - renderwidth;
	else if (!status.x and status.xr) status.xdistance = position.x;
	if      (!status.y and status.yu) status.ydistance = position.y + height * scale - renderheight;
	else if (!status.y and status.yd) status.ydistance = position.y - statusheight;

	return status;
}

SDL_Rect PositionComponent::getCenterPoint() {

	return {
		int(position.x) + width * scale / 2,
		int(position.y) + height * scale / 2
	};
}

Vector2D PositionComponent::getActualMovement() {

	return position - previousPosition;
}