#include "Engine.h"
#include "TextureManager.h"
#include "ECS/Components.h"
#include "BackgroundManager.h"
#include "LevelManager.h"
#include "Statusbar.h"
#include "AssetsManager.h"
#include "CollisionsManager.h"
#include <fstream>

BackgroundManager backgroundManager;
Statusbar statusbar;

SDL_Renderer* Engine::renderer;
SDL_Event* Engine::event;

Manager Engine::manager;
AssetsManager Engine::assets;
CollisionsManager Engine::collisions;

// Put the manager->groupedEntities vectors in the refereces variables to control
// the render layers in the render() metod

auto& backgrounds = Engine::manager.getGroup(groupBackgrounds);
auto& bullets =     Engine::manager.getGroup(groupBullets);
auto& enemies =     Engine::manager.getGroup(groupEnemies);
auto& players =     Engine::manager.getGroup(groupPlayer);
auto& explosions =  Engine::manager.getGroup(groupExplosions);
auto& pieces =      Engine::manager.getGroup(groupPieces);
auto& status =      Engine::manager.getGroup(groupStatus);

void Engine::init(const char* title, Window mWindow, bool fullscreen) {

	int flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

	if (fullscreen) {

		flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		window = SDL_CreateWindow(title, mWindow.xposition, mWindow.yposition, mWindow.width, mWindow.height, flags);

		if (window) {

			std::cout << "Window created succesfuly" << std::endl;
			SDL_SetWindowMinimumSize(window, 600, 350);
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

		if (renderer) {

			std::cout << "Renderer created successfuly" << std::endl;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderSetLogicalSize(renderer, renderwidth, renderheight);
		}
		
		if (TTF_Init() == 0) {

			std::cout << "SDL_ttf init succesfuly" << std::endl;
		}

		event = new SDL_Event();

		// Adding assets
		assets.addAssets("assetsList.json");

		// Adding levels to the background
		backgroundManager.addLevel("background1_level1", 2);

		// Load statusbar settings
		statusbar = Statusbar("statusbar//statusbarSettings.json");

		auto& player = assets.loadAsset("playerSpaceship", 30, 350);
		auto& entity2 = assets.loadAsset("asset2", 500, 200);

		// Uncommet this lines to omit the assetsManager for the player
		/*SDL_Rect playerPosition = {30, 350, 70, 30};
		auto& player = manager.addEntity();
		player.addComponent<PositionComponent>(playerPosition, 1);
		player.addComponent<TransformComponent>();
		player.addComponent<ColliderComponent>(playerId);
		player.addComponent<SpriteComponent>("assets//textures//spaceships//spaceship1.png");
		player.getComponent<SpriteComponent>().addAnimation("base", 4, 0, 100);
		player.getComponent<SpriteComponent>().addAnimation("shot", 4, 1, 100);
		player.getComponent<SpriteComponent>().addAnimation("damage", 4, 2, 100);
		player.addComponent<PlayerSpaceship>();
		player.addGroup(groupPlayer);*/

		statusbar.init();

		auto& energyWidget = statusbar.addWidget<EnergyWidget>(&player.getComponent<PlayerSpaceship>().energy);
		auto& missilesWidget = statusbar.addWidget<MissilesWidget>(&player.getComponent<PlayerSpaceship>().missiles);
		auto& weaponWidget = statusbar.addWidget<WeaponWidget>(&player.getComponent<PlayerSpaceship>());

		energyWidget.setModel("999");
		energyWidget.setColor(255, 255, 0, 180);
		energyWidget.setIcon("statusbar//icons//energyIcon.png", 12, 6, 5);

		missilesWidget.setModel("999");
		missilesWidget.setColor(255, 255, 0, 180);
		missilesWidget.setIcon("statusbar//icons//missileIcon.png", 12, 6, 5);

		weaponWidget.setModel("missile");
		weaponWidget.setColor(255, 255, 0, 180);
		weaponWidget.setPrefix("W:");

		std::cout << IMG_GetError() << std::endl;

		isRunning = true;
	}

	else {

		std::cout << SDL_GetError() << std::endl;
		isRunning = false;
	}
}

void Engine::update() {

	collisions.refresh();
	statusbar.refresh();
	manager.refersh();

	//levelManager.update();
	backgroundManager.update();
	statusbar.update();
	manager.update();

	collisions.update();
}

void Engine::render() {

	SDL_RenderClear(renderer);

	for (auto& t : backgrounds) t->draw();
	for (auto& t : pieces)      t->draw();
	for (auto& t : bullets)     t->draw();
	for (auto& t : enemies)     t->draw();
	for (auto& t : players)     t->draw();
	for (auto& t : explosions)  t->draw();
	for (auto& t : status)      t->draw();

	SDL_RenderPresent(renderer);
}

void Engine::events() {

	SDL_PollEvent(event);

	switch (event->type) {

	case SDL_QUIT:

		isRunning = false;
		break;

	case SDL_WINDOWEVENT:

		if (event->window.event == SDL_WINDOWEVENT_RESIZED) {

			float scaleFactorX = float(event->window.data1) / float(renderwidth);
			float scaleFactorY = float(event->window.data2) / float(renderheight);

			if (scaleFactorX < scaleFactorY)
				SDL_RenderSetScale(renderer, scaleFactorX, scaleFactorX);
			else
				SDL_RenderSetScale(renderer, scaleFactorY, scaleFactorY);
		}

		break;

	case SDL_KEYDOWN:

		if (event->key.keysym.sym == SDLK_F11) {

			int flags = SDL_GetWindowFlags(window);
			int fullscreen = flags&SDL_WINDOW_FULLSCREEN;

			if (fullscreen == SDL_WINDOW_FULLSCREEN)
				SDL_SetWindowFullscreen(window, !SDL_WINDOW_FULLSCREEN);
			else
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		}
		
		break;

	default:
		break;
	}
}

void Engine::clean() {
	
	SDL_DestroyRenderer(renderer);
	std::cout << "renderer destroyed successfuly" << std::endl;

	SDL_DestroyWindow(window);
	std::cout << "window destroyed successfuly" << std::endl;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool Engine::running() {

	return isRunning;
}