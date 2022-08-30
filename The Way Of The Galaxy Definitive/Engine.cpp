#include "Engine.h"
#include "TextureManager.h"
#include "Components.h"
#include "Background.h"
#include "Collision.h"
#include "nlohmann/json.hpp"
#include "ColliderIds.h"
#include "GroupLabels.h"
#include "LevelManager.h"
#include "Text.h"
#include <fstream>

using json = nlohmann::json;

Manager manager;
LevelManager levelmanager(manager);
SDL_Renderer* Engine::renderer;
SDL_Event* Engine::event;
int Engine::renderwidth = 0;
int Engine::renderheight = 0;

Background background;

auto& player = manager.addEntity();
auto& enemy = manager.addEntity();
Text text;

std::vector<ColliderComponent*> Engine::colliders;

// Put the manager->groupedEntities vectors in the refereces variables to control
// the render layers in the render() metod

auto& bullets = manager.getGroup(groupBullets);
auto& enemies = manager.getGroup(groupEnemies);
auto& players = manager.getGroup(groupPlayer);
auto& explosions = manager.getGroup(groupExplosions);

void Engine::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {

	int flags = SDL_WINDOW_RESIZABLE;

	if (fullscreen) {

		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderwidth = width;
		renderheight = height;

		if (window) {

			std::cout << "Window created succesfuly" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer) {

			std::cout << "Renderer created successfuly" << std::endl;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderSetLogicalSize(renderer, renderwidth, renderheight);
		}
		
		if (TTF_Init() == 0) {

			std::cout << "SDL_ttf init succesfuly" << std::endl;
		}

		event = new SDL_Event();

		// Adding wallpapers
		
		background = Background(640, 512, 2);
		background.addWallpaper("sprites\\backgrounds\\background1.png", 1);
		background.addWallpaper("sprites\\backgrounds\\background2.png", 2);
		//background.addWallpaper("sprites\\backgrounds\\background3.png", 4);

		// Setting components

		player.addComponent<PositionComponent>(30, 350, 70, 30, 1, 0, true);
		player.getComponent<PositionComponent>().setSpeed(3, 3);
		player.addComponent<SpriteComponent>("sprites\\spaceships\\spaceship1.png", true);
		player.getComponent<SpriteComponent>().addAnimation("base", 4, 0, 100);
		player.getComponent<SpriteComponent>().addAnimation("shot", 4, 1, 100);
		player.getComponent<SpriteComponent>().addAnimation("damage", 4, 2, 100);
		player.getComponent<SpriteComponent>().playAnimation("base");
		player.addComponent<PlayerComponent>();
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>(playerId);
		player.addGroup(groupPlayer);

		/*enemy.addComponent<PositionComponent>(500, 500, 70, 30, 1);
		enemy.addComponent<SpriteComponent>("sprites\\spaceships\\spaceship1.png", true);
		enemy.getComponent<SpriteComponent>().addAnimation("base", 4, 0, 100);
		enemy.getComponent<SpriteComponent>().playAnimation("base");
		enemy.addComponent<ColliderComponent>(enemyId);
		enemy.addComponent<EnemyComponent>();
		enemy.addGroup(groupEnemies);*/

		text = Text("sprites\\fonts\\character.ttf", 32, { 255, 255, 255 }, "Energy: 0", 8, -10, 32 * 11 / 2, 40);

		std::cout << IMG_GetError() << std::endl;

		levelmanager.startLevel("levelmaps\\test.json");
		isRunning = true;
	}

	else {

		isRunning = false;
	}
}

void Engine::update() {

	background.update();
	refreshColliders();
	levelmanager.update();
	manager.refersh();
	manager.update();
	text.setText(std::string("Energy: " + std::to_string(player.getComponent<PlayerComponent>().energy)).c_str());

	/*if (!player.getComponent<PositionComponent>().isCompletelyOnRender()) {

		player.getComponent<PositionComponent>().restorePosition();
	}*/

	for (auto cc : colliders) {

		switch (cc->id) {

		case playerId:

			for (auto cd : colliders) {
				
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

			for (auto cd : colliders) {

				if (Collision::AABB(*cc, *cd)) {

					switch (cd->id) {

					case enemyId:

						std::cout << "Collision laser->enemy" << std::endl;
						cd->entity->getComponent<EnemyComponent>().explode();
						cc->entity->destroy();
						break;
					}
				}
			}

			break;

		case enemylaserId:

			for (auto cd : colliders) {

				if (Collision::AABB(*cc, *cd)) {

					switch (cd->id) {

					case playerId:

						std::cout << "Collision enemy laser->player" << std::endl;
						cc->entity->destroy();
						cd->entity->getComponent<PlayerComponent>().energy -= 2;
					}
				}
			}
		}
	}
}

void Engine::render() {

	SDL_RenderClear(renderer);
	background.draw();
	//manager.draw();

	for (auto& t : bullets) t->draw();
	for (auto& t : enemies) t->draw();
	for (auto& t : players) t->draw();
	for (auto& t : explosions) t->draw();

	text.draw();
	SDL_RenderPresent(renderer);
}

void Engine::closeEvent() {

	SDL_PollEvent(event);
	switch (event->type) {

	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

void Engine::refreshColliders() {

	colliders.erase(std::remove_if(std::begin(colliders), std::end(colliders), [](ColliderComponent* mCollider) {

		return !mCollider->entity->isActive();
	}),

	std::end(colliders));
}

void Engine::clean() {
	
	SDL_DestroyRenderer(renderer);
	std::cout << "renderer destroyed successfuly" << std::endl;
	SDL_DestroyWindow(window);
	std::cout << "window destroied successfuly" << std::endl;
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}