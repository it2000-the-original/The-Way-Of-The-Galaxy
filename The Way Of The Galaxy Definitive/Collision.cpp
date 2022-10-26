#include "ColliderComponent.h"
#include "Collision.h"

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

void Collision::checkCollisions() {

	for (auto cc : Engine::colliders) {

		switch (cc->id) {

		case playerId:

			for (auto cd : Engine::colliders) {

				if (AABB(*cc, *cd)) {

					switch (cd->id) {

					case enemyId:

						std::cout << "Collision player->enemy" << std::endl;
						break;
					}
				}
			}
			
			break;

		case laserId:

			for (auto cd : Engine::colliders) {

				if (AABB(*cc, *cd)) {

					switch (cd->id) {

					case enemyId:

						std::cout << "Collision laser->enemy" << std::endl;
						cc->entity->destroy();
						cd->entity->getComponent<ExplodeComponent>().explode();
						break;
					}
				}
			}

			break;

		case enemylaserId:

			for (auto cd : Engine::colliders) {

				if (AABB(*cc, *cd)) {

					switch (cd->id) {

					case playerId:

						std::cout << "Collision enemy laser->player" << std::endl;
						cc->entity->destroy();
						cd->entity->getComponent<PlayerComponent>().energy -= 1;
						break;
					}
				}
			}

			break;

		case missileId:

			for (auto cd : Engine::colliders) {

				if (AABB(*cc, *cd)) {

					switch (cd->id) {

					case enemyId:

						std::cout << "Collision missile->enemy" << std::endl;
						cc->entity->getComponent<ExplodeComponent>().explode();
						cd->entity->getComponent<ExplodeComponent>().explode();
						break;
					}
				}
			}

			break;
		}

	}
}