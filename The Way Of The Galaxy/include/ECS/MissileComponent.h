#pragma once
#include "ECS.h"

class PositionComponent;
class SpriteComponent;
class Vector2D;

class MissileComponent : public Component {
	
private:

	PositionComponent* position;
	SpriteComponent* sprite;
	Vector2D acceleration = Vector2D(0.25, 0);

public:

	int damage;

	MissileComponent();
	MissileComponent(int mDamage);
	MissileComponent(int mDamage, float mAcceleration);

	void init() override;
	void update() override;

	void setAcceleration(float mAcceleration);
};
