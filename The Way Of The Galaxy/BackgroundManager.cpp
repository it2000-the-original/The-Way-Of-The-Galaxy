#include "BackgroundManager.h"
#include "AssetsManager.h"

void BackgroundManager::addLevel(std::string assetName, int speed) {

	Level level;

	Asset asset = Engine::assets.getAsset(assetName);

	int xrepeat = renderwidth / (asset.width * asset.scale) + 2;
	int yrepeat = renderheight / (asset.height * asset.scale) + 1;

	int actualXposition = 0;
	int actualYposition = statusheight;
	
	for (int i = 0; i < xrepeat; i++) {

		for (int j = 0; j < yrepeat; j++) {

			auto* entity = &Engine::assets.loadAsset(assetName, actualXposition, actualYposition);
			entity->getComponent<PositionComponent>().setSpeed(-speed, 0);

			level.entities.emplace_back(entity);
			actualYposition += asset.height * asset.scale;
		}
		
		actualYposition = 0;
		actualXposition += asset.width * asset.scale;
	}

	level.xrepeat = xrepeat;
	level.yrepeat = yrepeat;

	background.emplace_back(level);

	std::cout << "Added background level " << assetName << std::endl;
}

void BackgroundManager::update() {

	// If an entity of a level is outside of the render area,
	// it will be moved to the back of the last entity

	for (auto level : background) {

		for (auto entity : level.entities) {

			if (!entity->getComponent<PositionComponent>().isOnRender().xl) {

				int entityWidth = entity->getComponent<PositionComponent>().width;
				int entityScale = entity->getComponent<PositionComponent>().scale;

				entity->getComponent<PositionComponent>().position.x += entityWidth * entityScale * level.xrepeat;
			}
		}
	}
}

void BackgroundManager::clear() {

	// Destroy every entity of every level and clear the background

	for (auto level : background) {

		for (auto entity : level.entities) {

			entity->destroy();
		}
	}

	background.clear();
}