#pragma once
#include <array>
#include "Components.h"
#include "Vector2D.h"
#include "Engine.h"

struct statusPosition {

	bool x = false, y = false,
	xr = false, xl = false,
	yu = false, yd = false,
	xy = false;
};

class PositionComponent : public Component {

//private:


public:

	Vector2D backupPosition;
	Vector2D position;
	Vector2D velocity;

	int rotationSpeed;
	bool controlled;
	int height;
	int width;

	int angle;

	int scale;

	PositionComponent() {

		position.Zero();
		width = height = 32;
		controlled = false;
		rotationSpeed = 0;
		angle = 0;
		scale = 1;
	}

	PositionComponent(int x, int y) {

		position.x = x;
		position.y = y;
		width = height = 32;
		controlled = false;
		rotationSpeed = 0;
		angle = 0;
		scale = 1;
	}

	PositionComponent(int x, int y, int w, int h, int s) {

		position.x = x;
		position.y = y;
		controlled = false;
		rotationSpeed = 0;
		width = w;
		height = h;
		angle = 0;
		scale = s;
	}

	PositionComponent(int x, int y, int w, int h, int s, int a) {

		position.x = x;
		position.y = y;
		controlled = false;
		rotationSpeed = 0;
		width = w;
		height = h;
		angle = a;
		scale = s;
	}

	PositionComponent(int x, int y, int w, int h, int s, int a, int c) {

		position.x = x;
		position.y = y;
		controlled = c;
		rotationSpeed = 0;
		width = w;
		height = h;
		angle = a;
		scale = s;
	}

	PositionComponent(int x, int y, int w, int h, int s, int a, int c, int rs) {

		position.x = x;
		position.y = y;
		controlled = c;
		rotationSpeed = rs;
		width = w;
		height = h;
		angle = a;
		scale = s;
	}

	~PositionComponent() {}

	void init() override {

		velocity.Zero();
	}

	void update() override {

		backupPosition = position;
		angle += rotationSpeed;

		if (!controlled) {

			position += velocity;
		}
	}

	void setSpeed(int x, int y) {

		velocity.x = x;
		velocity.y = y;
	}

	void setPosition(int x, int y) {

		position.x = x;
		position.y = y;
	}

	void restorePosition(bool x, bool y) {

		if (x) position.x = backupPosition.x;
		if (y) position.y = backupPosition.y;
	}

	statusPosition isOnRender() {

		statusPosition status;

		if (position.x > -width * scale) status.xl = true;
		if (position.x < Engine::renderwidth) status.xr = true;
		if (status.xl == true and status.xr == true) status.x = true;

		if (position.y > -height * scale) status.yu = true;
		if (position.y < Engine::renderheight) status.yd = true;
		if (status.yu == true and status.yd == true) status.y = true;

		if (status.x == true and status.y == true) status.xy = true;

		return status;
	}

	std::array<bool, 2> isCompletelyOnRender() {

		std::array<bool, 2> xyarray;
		
		if (position.x >= 0 and	position.x <= Engine::renderwidth - width * scale) xyarray[0] = true;
		else xyarray[0] = false;

		if (position.y >= 0 and position.y <= Engine::renderheight - height * scale) xyarray[1] = true;
		else xyarray[1] = false;

		return xyarray;
	}

	SDL_Rect getCenterPoint() {

		return { position.x + width * scale / 2, position.y + height * scale / 2, 0, 0 };
	}
};