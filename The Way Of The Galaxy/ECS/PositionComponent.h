#pragma once
#include "Components.h"
#include "Vector2D.h"
#include "Engine.h"

struct statusPosition {

	bool x = false, y = false,
	xr = false, xl = false,
	yu = false, yd = false,
	xy = false;

	int xdistance = 0;
	int ydistance = 0;
};

class PositionComponent : public Component {

private:

	Vector2D controlledVelocity = Vector2D();
	bool controlled = false;

	Vector2D previousPosition = Vector2D();
	int rotationSpeed = 0;

public:

	Vector2D speed = Vector2D();
	Vector2D velocity = Vector2D();
	Vector2D position = Vector2D();

	int height = 64;
	int width = 64;
	int angle = 0;
	int scale = 1;

	PositionComponent() {}

	PositionComponent(int x, int y) {

		position.x = x;
		position.y = y;
	}

	PositionComponent(SDL_Rect space, int s) {

		position.x = space.x;
		position.y = space.y;

		width = space.w;
		height = space.h;
		scale = s;
	}

	PositionComponent(SDL_Rect space, int s, int a) {

		position.x = space.x;
		position.y = space.y;

		width = space.w;
		height = space.h;
		scale = s;
		angle = a;
	}

	PositionComponent(SDL_Rect space, int s, int a, int c) {

		position.x = space.x;
		position.y = space.y;

		width = space.w;
		height = space.h;
		scale = s;
		angle = a;

		controlled = c;
	}

	void init() override {

		velocity.Zero();
	}

	void update() override {

		previousPosition = position;
		angle += rotationSpeed;
		position += velocity;
	}

	void setPosition(int x, int y) {

		position.x = x;
		position.y = y;
	}

	void setSpeed(int x, int y) {

		velocity.x = x;
		velocity.y = y;
	}

	void setRotationSpeed(int mRotationSpeed) {

		rotationSpeed = mRotationSpeed;
	}

	void setControlledSpeed(int x, int y) {

		if (controlled) {

			controlledVelocity.x = x;
			controlledVelocity.y = y;
		}
	}

	statusPosition isOnRender() {

		// This function return a status struct to know if the entity is in
		// the render area and to get distance to the margin in x and y

		statusPosition status;

		if (position.x >= -width * scale) status.xl = true;
		if (position.x <= renderwidth) status.xr = true;
		if (status.xl == true and status.xr == true) status.x = true;

		if (position.y >= -height * scale + statusheight) status.yu = true;
		if (position.y <= renderheight) status.yd = true;
		if (status.yu == true and status.yd == true) status.y = true;

		if (status.x == true and status.y == true) status.xy = true;

		if      (!status.x and status.xl) status.xdistance = position.x - renderwidth;
		else if (!status.x and status.xr) status.xdistance = position.x + width * scale;
		if      (!status.y and status.yu) status.ydistance = position.y - renderheight;
		else if (!status.y and status.yd) status.ydistance = position.y + height * scale - statusheight;

		return status;
	}

	statusPosition isCompletelyOnRender() {
		
		// This function return a status struct to know if the entity 
		// is completely with all the space of the sprite in
		// the render area and to get distance to the margin in x and y

		statusPosition status;

		if (position.x >= 0) status.xl = true;
		if (position.x <= renderwidth - width * scale) status.xr = true;
		if (status.xl == true and status.xr == true) status.x = true;

		if (position.y >= statusheight) status.yu = true;
		if (position.y <= renderheight - height * scale) status.yd = true;
		if (status.yu == true and status.yd == true) status.y = true;

		if (status.x == true and status.y == true) status.xy = true;

		if      (!status.x and status.xl) status.xdistance = position.x + width * scale - renderwidth;
		else if (!status.x and status.xr) status.xdistance = position.x;
		if      (!status.y and status.yu) status.ydistance = position.y + height * scale - renderheight;
		else if (!status.y and status.yd) status.ydistance = position.y - statusheight;

		return status;
	}

	SDL_Rect getCenterPoint() {

		return { 
			position.x + width * scale / 2,
			position.y + height * scale / 2 
		};
	}

	Vector2D getActualMovement() {

		return position - previousPosition;
	}

	void moveRight() {position.x += controlledVelocity.x;}
	void moveLeft()  {position.x -= controlledVelocity.x;}
	void moveUp()    {position.y -= controlledVelocity.y;}
	void moveDown()  {position.y += controlledVelocity.y;}
};