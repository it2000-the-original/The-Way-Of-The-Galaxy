#pragma once
#include <array>
#include "Components.h"
#include "Vector2D.h"
#include "Engine.h"

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

	void restorePosition(bool x, bool y) {

		if (x) position.x = backupPosition.x;
		if (y) position.y = backupPosition.y;
	}

	bool isOnRender() {

		if (

			position.x > -width and
			position.y > -height and
			position.x < Engine::renderwidth and
			position.y < Engine::renderheight

			) {

			return true;
		}

		return false;
	}

	std::array<bool, 2> isCompletelyOnRender() {

		std::array<bool, 2> xyarray;
		
		if (position.x >= 0 and	position.x <= Engine::renderwidth - width) xyarray[0] = true;
		else xyarray[0] = false;

		if (position.y >= 0 and position.y <= Engine::renderheight - height) xyarray[1] = true;
		else xyarray[1] = false;

		return xyarray;
	}
};