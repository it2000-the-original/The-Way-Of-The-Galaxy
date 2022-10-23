#pragma once
#include <SDL2/SDL.h>
#include "Engine.h"
#include "TextureManager.h"
#include "Components.h"
#include <vector>

class BackgroundsManager {

private:

	std::vector<std::vector<Entity*>> backgrounds;
	Manager& manager;

public:

	BackgroundsManager(Manager& mManager);
	void addWallpaper(const char* path, int speed, int entities, int width, int height, int scale);
	void update();
};