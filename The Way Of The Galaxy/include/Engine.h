#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

class AssetsManager;

const int statusheight = 30;
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
	static AssetsManager* assets;

	void init(const char* title, Window mWindow, bool fullscreen);

	void update();
	void clean();
	void render();
	void closeEvent();
	bool running();
};
