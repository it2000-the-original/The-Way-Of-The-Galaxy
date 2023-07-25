#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

class Manager;
class AssetsManager;
class CollisionsManager;

inline int statusheight = 0;

const int renderwidth = 1200;
const int renderheight = 700;

enum ColliderIds : std::size_t {

	playerId,
	enemyId,
	laserId,
	enemylaserId,
	missileId,
	satId
};

enum GroupLabels : std::size_t {

	groupBullets,
	groupEnemies,
	groupPlayer,
	groupExplosions,
	groupPieces,
	groupBackgrounds,
	groupStatus
};

enum ComponentIds : std::size_t {

	playerSpaceship,
	missile
};

struct Window {

	int xposition = 100;
	int yposition = 100;
	int width = 600;
	int height = 300;
};

class Engine {

private:

	SDL_Window* window;
	bool isRunning;

public:

	static SDL_Renderer* renderer;
	static SDL_Event* event;

	static Manager manager;
	static AssetsManager assets;
	static CollisionsManager collisions;

	void init(const char* title, Window mWindow, bool fullscreen);

	void update();
	void clean();
	void render();
	void events();
	bool running();
};
