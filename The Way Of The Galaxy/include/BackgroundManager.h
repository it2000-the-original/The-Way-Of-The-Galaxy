#pragma once
#include <SDL2/SDL.h>
#include "Engine.h"
#include "TextureManager.h"
#include "Components.h"
#include <vector>

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

	Background background;

public:

	void addLevel(std::string assetName, int speed);
	void clear();
	void update();
};