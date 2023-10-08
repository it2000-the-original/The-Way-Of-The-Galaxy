#pragma once

// This is similar to SDL_Rect, but width double variables.
// The reason is that I need more precision to manage collisions and other

struct Rectangle {

	double x, y;
	double w, h;
};
