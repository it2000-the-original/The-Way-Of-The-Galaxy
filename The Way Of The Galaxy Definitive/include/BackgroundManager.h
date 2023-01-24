#pragma once
#include <SDL2/SDL.h>
#include "Engine.h"
#include "TextureManager.h"
#include "Components.h"
#include <vector>

// Struct that define a srolling 
// level of the background

struct LevelBackground {

	const char* path;
	int speed = 5;
	int width = 100;
	int height = 100;
	int scale = 1;
};

// Struct that define a level 
// of the background

struct Level {

	std::vector<Entity*> entities;
	int xrepeat;
	int yrepeat;
};

using Background = std::vector<Level>;

class BackgroundManager {

private:

	Manager& manager;
	Background background;

public:

	BackgroundManager(Manager& mManager);
	void addLevel(LevelBackground level);
	void clear();
	void update();
};