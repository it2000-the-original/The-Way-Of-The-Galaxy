#pragma once
#include <functional>
#include "Collision.h"
#include "Engine.h"

class ColliderComponent;

void printHello();
void checkForCollisions(ColliderComponent* collider, std::size_t id, std::function<void()> func);
void subtractCollisionMTV(ColliderComponent* collider, std::size_t id);
void checkCollisions();
