#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {

	if (
		recA.x + recA.w >= recB.x and
		recB.x + recB.w >= recA.x and
		recA.y + recA.h >= recB.y and
		recB.y + recB.h >= recA.y
		) {

		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent colA, const ColliderComponent colB) {

	if (AABB(colA.collider, colB.collider)) {

		return true;
	}

	return false;
}