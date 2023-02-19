
//   _______ _        __           __            ____   __   _______ _             _____       _
//  |__   __| |       \ \         / /           / __ \ / _| |__   __| |           / ____|     | |
//     | |  | |__   ___\ \  / \  / /_ _ _   _  | |  | | |_     | |  | |__   ___  | |  __  __ _| | __ ___  ___   _
//     | |  | '_ \ / _ \\ \/   \/ / _` | | | | | |  | |  _|    | |  | '_ \ / _ \ | | |_ |/ _` | |/ _` \ \/ / | | |
//     | |  | | | |  __/ \  / \  / (_| | |_| | | |__| | |      | |  | | | |  __/ | |__| | (_| | | (_| |>  <| |_| |
//     |_|  |_| |_|\___|  \/   \/ \__,_|\__, |  \____/|_|      |_|  |_| |_|\___|  \_____|\__,_|_|\__,_/_/\_\\__, |
//                                       __/ |                                                               __/ |
//                                      |___/                                                               |___/ 

#include <iostream>
#include "Engine.h"
#include <time.h>

const int FPS = 60;
const Uint32 frameDelay = 1000 / FPS;
const char* windowTitle = "The Way Of The Galaxy";

Window window = {
	SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED,
	1200, 700
};

int main(int argc, char* argv[]) {

	Uint32 frameStart;
	Uint32 frameTime;
	
	// Initialize the random functions
	srand(time(NULL));

	Engine* engine = new Engine();

	engine->init(windowTitle, window, false);

	while (engine->running()) {

		frameStart = SDL_GetTicks();

		engine->update();
		engine->render();
		engine->close();

		frameTime = SDL_GetTicks() - frameStart;
		
		// Limiting the frameate to 60 fps

		if (frameDelay > frameTime) {

			SDL_Delay(frameDelay - frameTime);
		}
	}

	engine->clean();

	return 0;
}