#include "CollisionSettings.h"

void checkCollisions() {

	for (auto cc : Engine::colliders) {

		switch (cc->id) {

		case playerId:

			for (auto cd : Engine::colliders) {

				if (Collision::AABB(*cc, *cd)) {

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

				if (Collision::AABB(*cc, *cd)) {

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

				if (Collision::AABB(*cc, *cd)) {

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

				if (Collision::AABB(*cc, *cd)) {

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