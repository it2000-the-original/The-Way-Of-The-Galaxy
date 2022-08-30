#include <iostream>
#include "Engine.h"

Engine* engine = NULL;

int main(int argc, char* argv[]) {

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	
	engine = new Engine();
	engine->init("The Way Of The Galaxy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 700, false);

	while (engine->running()) {

		frameStart = SDL_GetTicks();
		engine->update();
		engine->render();
		engine->closeEvent();
		frameTime = SDL_GetTicks() - frameStart;
		
		if (frameDelay > frameTime) {

			SDL_Delay(frameDelay - frameTime);
		}
	}

	engine->clean();

	return 0;
}