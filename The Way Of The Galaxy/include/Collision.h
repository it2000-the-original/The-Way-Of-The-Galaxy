#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <functional>

using Function = std::function<void()>;

class ColliderComponent;
class Vector2D;

class Collision {

public:

	static std::vector<ColliderComponent*> colliders;

	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent colA, const ColliderComponent colB);
	static bool SAT(const ColliderComponent colA, const ColliderComponent colB);
	static Vector2D MTV(const ColliderComponent colA, const ColliderComponent colB);

	static void checkCollisionSAT(ColliderComponent* collider, std::size_t id, Function func);
	static void subtractCollisionMTV(ColliderComponent* collider, std::size_t id);
	static void refreshColliders();
};