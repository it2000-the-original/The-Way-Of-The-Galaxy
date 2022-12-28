#pragma once
#include <SDL2/SDL.h>

class ColliderComponent;
class Vector2D;

class Collision {

public:

	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent colA, const ColliderComponent colB);
	static bool SAT(const ColliderComponent colA, const ColliderComponent colB);
	static Vector2D MTV(const ColliderComponent colA, const ColliderComponent colB);
};