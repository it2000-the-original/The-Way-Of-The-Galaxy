#pragma once
#ifndef Engine_h
#define Engine_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

class ColliderComponent;

const int statusheight = 0;
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

class Engine {

public:

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void update();
	void render();
	void refreshColliders();
	void closeEvent();
	void clean();
	bool running() { return isRunning; }

	static SDL_Renderer* renderer;
	static SDL_Event* event;
	static std::vector<ColliderComponent*> colliders;

private:

	SDL_Window* window;
	bool isRunning;
};


#endif
