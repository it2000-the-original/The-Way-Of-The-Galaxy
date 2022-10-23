#include "BackgroundsManager.h"
#include "GroupLabels.h"

BackgroundsManager::BackgroundsManager(Manager& mManager) : manager(mManager) {}

void BackgroundsManager::addWallpaper(const char* path, int speed, int entitiesNumber, int width, int height, int scale) {

	int actualPosition = 0;

	std::vector<Entity*> entities;
	
	for (int i = 0; i < entitiesNumber; i++) {

		entities.emplace_back(&manager.addEntity());
		entities[i]->addComponent<PositionComponent>(actualPosition, 0, width, height, scale);
		entities[i]->getComponent<PositionComponent>().setSpeed(-speed, 0);
		entities[i]->addComponent<SpriteComponent>(path);
		entities[i]->addGroup(groupBackgrounds);
		actualPosition += width * scale;
	}

	backgrounds.emplace_back(entities);
}

void BackgroundsManager::update() {

	for (auto vec : backgrounds)
		for (auto* ent : vec) {

			if (!ent->getComponent<PositionComponent>().isOnRender().xl) {

				ent->getComponent<PositionComponent>().position.x += (ent->getComponent<PositionComponent>().width * ent->getComponent<PositionComponent>().scale) * vec.size();
			}
		}
}