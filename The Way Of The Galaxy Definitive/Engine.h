#pragma once
#ifndef Engine_h
#define Engine_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

class ColliderComponent;

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
	static int renderwidth;
	static int renderheight;
	static std::vector<ColliderComponent*> colliders;

private:

	SDL_Window* window;
	bool isRunning;
};


#endif