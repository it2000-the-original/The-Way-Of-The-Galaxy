#pragma once
#include "ECS.h"

class PositionComponent;
class Vector2D;

class TransformComponent : public Component {

private:

	PositionComponent* position;

	Vector2D velocity = Vector2D(1, 1);

	// Variable to comfirm that precedently a
	// function to move was called

	bool rxmoved = false;
	bool lxmoved = false;
	bool uymoved = false;
	bool dymoved = false;

	Vector2D externalMotion = Vector2D(0, 0);

	Vector2D acceleration = Vector2D(1, 1);

public:

	TransformComponent();
	TransformComponent(Vector2D mVelocity, Vector2D mAcceleration);

	void init() override;
	void update() override;

	void setAcceleration(Vector2D mAcceleration);
	void setVelocity(Vector2D mVelocity);

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void setExternalMotion(Vector2D motion);
	void removeVelocityExcesses();
	void xAxisDecelleration();
	void yAxisDecelleration();
	void linearDecelleration();
};
