#pragma once
#include "SDL.h"
#include "Engine.h"
#include "TextureManager.h"
#include <vector>

struct Wallpaper {

	SDL_Rect destRect1;
	SDL_Rect destRect2;
	SDL_Texture* wallpaperTexture;
	int wallpaperSpeed;
};

class Background {

private:

	SDL_Rect srcRect;
	int backgroundScale;
	std::vector<Wallpaper> wallpapers;

public:

	Background();
	Background(int width, int height, int scale);
	void update();
	void draw();
	void addWallpaper(const char* path, int speed);
};