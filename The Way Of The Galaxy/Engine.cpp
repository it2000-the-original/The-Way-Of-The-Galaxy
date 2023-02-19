#include "Engine.h"
#include "TextureManager.h"
#include "Components.h"
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

// Defining levels of the background

LevelBackground bg1 = { "sprites//backgrounds//Space_Stars2.png", 2, 64, 64, 2 };

// Defining initial positions of the entities

SDL_Rect playerPosition = { 30, 350, 70, 30 };

// Defining the statusbar

Status statusSetting = {
	"sprites//statusbar.png",
	"sprites//fonts//pixelfonts.ttf",
	statusheight,
	20, 4, 20
};

void Engine::init(const char* title, Window mWindow, bool fullscreen) {

	int flags = SDL_WINDOW_RESIZABLE;

	if (fullscreen) {

		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		window = SDL_CreateWindow(title, mWindow.xposition, mWindow.yposition, mWindow.width, mWindow.height, flags);

		if (window) {

			std::cout << "Window created succesfuly" << std::endl;
		}

		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
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

		// Addind levels to the background
		backgroundManager.addLevel(bg1);

		auto& player = manager.addEntity();

		player.addComponent<PositionComponent>(playerPosition, 1, 0, true);
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>(playerId);
		player.addComponent<SpriteComponent>("sprites//spaceships//spaceship1.png");
		player.addComponent<PlayerSpaceship>();

		player.getComponent<PositionComponent>().setControlledSpeed(3, 3);
		player.getComponent<SpriteComponent>().addAnimation("base", 4, 0, 100);
		player.getComponent<SpriteComponent>().addAnimation("shot", 4, 1, 100);
		player.getComponent<SpriteComponent>().addAnimation("damage", 4, 2, 100);
		player.getComponent<SpriteComponent>().playAnimation("base");

		player.addGroup(groupPlayer);

		statusbar.init(statusSetting, true);
		statusbar.setAnimation(43, 0, 40);

		auto& energyWidget = statusbar.addWidget<EnergyWidget>(&player.getComponent<PlayerSpaceship>().energy);
		auto& missilesWidget = statusbar.addWidget<MissilesWidget>(&player.getComponent<PlayerSpaceship>().missiles);
		auto& weaponWidget = statusbar.addWidget<WeaponWidget>(&player.getComponent<PlayerSpaceship>());

		energyWidget.setModel("999");
		energyWidget.setColor(255, 255, 0, 180);
		energyWidget.setIcon("sprites//icons//energyIcon.png", 12, 6, 5);

		missilesWidget.setModel("999");
		missilesWidget.setColor(255, 255, 0, 180);
		missilesWidget.setIcon("sprites//icons//missileIcon.png", 12, 6, 5);

		weaponWidget.setModel("missile");
		weaponWidget.setColor(255, 255, 0, 180);
		weaponWidget.setPrefix("W: ");

		//assets->addAsset("asset", "assets//asset.json");
		assets.addAsset("asset2", "assets//asset2.json");

		std::cout << IMG_GetError() << std::endl;

		//auto& entity = assets->loadAsset("asset", 100, 100);
		auto& entity2 = assets.loadAsset("asset2", 500, 200);

		//entity.getComponent<SpriteComponent>().playAnimation("base");

		isRunning = true;
	}

	else {

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

void Engine::close() {

	SDL_PollEvent(event);

	switch (event->type) {

	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
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

bool Engine::running() {

	return isRunning;
}