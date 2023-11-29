#pragma once
#include <SDL2/SDL.h>
#include "Vector2D.h"
#include "Point.h"
#include <vector>

class ColliderComponent;

struct Collision2D {

	bool collision = false;
	Vector2D penetration;
	std::size_t colliderA;
	std::size_t colliderB;
	Vector2D colliderAVelocity;
	Vector2D colliderBVelocity;
};

class CollisionsManager {

public:

	std::vector<ColliderComponent*> colliders;

	Collision2D AABB(ColliderComponent* colA, ColliderComponent* colB);
	Collision2D SAT(ColliderComponent* colA, ColliderComponent* colB);
	Collision2D areInCollision(ColliderComponent* colA, ColliderComponent* colB);

	void update();
	void refresh();
};