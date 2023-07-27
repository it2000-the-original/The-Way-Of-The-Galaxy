#pragma once
#include <SDL2/SDL_image.h>

// A struct created to reduce the number of parameters to pass
// to the ExplodeComponent constructor

struct Explosion {

	SDL_Texture* texture;
	int frames = 10;
	int duration = 50;
	int width = 32;
	int height = 32;
};
