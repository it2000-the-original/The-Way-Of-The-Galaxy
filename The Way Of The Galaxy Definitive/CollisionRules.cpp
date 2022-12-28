#include "CollisionRules.h"
#include "Components.h"

void checkForCollisions(ColliderComponent* collider, std::size_t id, std::function<void()> func) {

	for (auto cc : Engine::colliders) {

		if (cc->id == id) {

			if (Collision::SAT(*collider, *cc)) func();
		}
	}
}


void getCollisionMTV(ColliderComponent* collider, std::size_t id) {

	for (auto cc : Engine::colliders) {

		if (cc->id == id) {

			collider->entity->getComponent<PositionComponent>().position -= Collision::MTV(*collider, *cc);
		}
	}
}

void printHello() {

	std::cout << "Hello" << std::endl;
}

void checkCollisions() {

	//checkForCollisions(playerId, satId, printHello);
}