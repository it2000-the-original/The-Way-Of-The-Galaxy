#include "BackgroundManager.h"

BackgroundManager::BackgroundManager(Manager& mManager) : manager(mManager) {}

void BackgroundManager::addLevel(LevelBackground sprite) {

	Level level;

	int xrepeat = renderwidth / (sprite.width * sprite.scale) + 2;
	int yrepeat = renderheight / (sprite.height * sprite.scale) + 1;

	int actualXposition = NULL;
	int actualYposition = statusheight;
	
	for (int i = 0; i < xrepeat; i++) {

		for (int j = 0; j < yrepeat; j++) {

			SDL_Rect backgroundSpace = { actualXposition, actualYposition, sprite.width, sprite.height };

			auto* entity = &manager.addEntity();

			entity->addComponent<PositionComponent>(backgroundSpace, sprite.scale);
			entity->addComponent<SpriteComponent>(sprite.path);
			entity->getComponent<PositionComponent>().setSpeed(-sprite.speed, 0);
			entity->addGroup(groupBackgrounds);

			level.entities.emplace_back(entity);
			actualYposition += sprite.height * sprite.scale;
		}
		
		actualYposition = NULL;
		actualXposition += sprite.width * sprite.scale;
	}

	level.xrepeat = xrepeat;
	level.yrepeat = yrepeat;

	background.emplace_back(level);
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