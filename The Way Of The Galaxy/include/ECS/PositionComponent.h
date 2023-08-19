#pragma once
#include <SDL2/SDL.h>
#include "ECS.h"

class Vector2D;

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

	PositionComponent();
	PositionComponent(int x, int y);
	PositionComponent(SDL_Rect space, int s);
	PositionComponent(SDL_Rect space, int s, int a);

	void init() override;
	void update() override;

	void setPosition(int x, int y);
	void setSpeed(int x, int y);
	void setRotationSpeed(int mRotationSpeed);
	statusPosition isOnRender();
	statusPosition isCompletelyOnRender();
	Point getCenterPoint();
	Vector2D getActualMovement();
	SDL_Rect getVisualRectangle();
};