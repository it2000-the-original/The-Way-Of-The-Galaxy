#pragma once
#include "Engine.h"
#include "Vector2D.h"

struct statusPosition {

	bool x  = false;
	bool y  = false;
	bool xr = false;
	bool xl = false;
	bool yu = false;
	bool yd = false;
	bool xy = false;

	float xdistance = 0;
	float ydistance = 0;
};

class PositionComponent : public Component {

private:

	Vector2D previousPosition = Vector2D();

public:

	Vector2D velocity = Vector2D();
	Vector2D position = Vector2D();

	int height = 64;
	int width = 64;
	int angle = 0;
	int scale = 1;

	int rotationSpeed = 0;

	PositionComponent() {}

	PositionComponent(int x, int y) {

		position.x = float(x);
		position.y = float(y);
	}

	PositionComponent(SDL_Rect space, int s) {

		position.x = float(space.x);
		position.y = float(space.y);

		width = space.w;
		height = space.h;
		scale = s;
	}

	PositionComponent(SDL_Rect space, int s, int a) {

		position.x = float(space.x);
		position.y = float(space.y);

		width = space.w;
		height = space.h;
		scale = s;
		angle = a;
	}

	void init() override {

		velocity.Zero();
	}

	void update() override {

		position.Round(3);
		velocity.Round(3);

		previousPosition = position;
		angle += rotationSpeed;
		position += velocity;
	}

	void setPosition(int x, int y) {

		position.x = float(x);
		position.y = float(y);
	}

	void setSpeed(int x, int y) {

		velocity.x = float(x);
		velocity.y = float(y);
	}

	void setRotationSpeed(int mRotationSpeed) {

		rotationSpeed = mRotationSpeed;
	}

	statusPosition isOnRender() {

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

	statusPosition isCompletelyOnRender() {
		
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

	SDL_Rect getCenterPoint() {

		return { 
			int(position.x) + width * scale / 2,
			int(position.y) + height * scale / 2 
		};
	}

	Vector2D getActualMovement() {

		return position - previousPosition;
	}
};