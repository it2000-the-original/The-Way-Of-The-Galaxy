#pragma once
#include <SDL2/SDL.h>
#include "Rectangle.h"
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

	double xdistance = 0;
	double ydistance = 0;
};

class PositionComponent : public Component {

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
	PositionComponent(Rectangle space, int s);
	PositionComponent(Rectangle space, int s, int a);

	void init() override;
	void update() override;

	void setPosition(double x, double y);
	void setSpeed(double x, double y);
	void setRotationSpeed(int mRotationSpeed);
	statusPosition isOnRender();
	statusPosition isCompletelyOnRender();
	Point getCenterPoint();
	Rectangle getVisualRectangle();
};