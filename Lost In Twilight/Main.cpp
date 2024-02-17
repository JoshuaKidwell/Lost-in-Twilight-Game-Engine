#include <SDL.h>
#include "GameEngine.h"
#include <iostream>

const int DESIRED_FPS = 60;

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	int fps = DESIRED_FPS;
	int frameDelay = 1000 / fps;
	int startTick;

	GameEngine LostInTwilight(&fps);

	while (LostInTwilight.IsRunning()) {
		if (fps > 0) {
			frameDelay = 1000 / fps;
		}
		startTick = SDL_GetTicks();
		////////////////////////////////////////

		LostInTwilight.Update();
		std::cout << "(" << fps << ")";

		////////////////////////////////////////
		if (SDL_GetTicks() - startTick < frameDelay) {
			if (fps < DESIRED_FPS) {
				if (DESIRED_FPS > 1000 / (SDL_GetTicks() - startTick + 1)) {
					fps = 1000 / (SDL_GetTicks() - startTick + 1);
				}
				else {
					fps = DESIRED_FPS;
				}
			}
			SDL_Delay(frameDelay - (SDL_GetTicks() - startTick));
		}
		else if (SDL_GetTicks() - startTick > frameDelay) {
			fps = 1000 / (SDL_GetTicks() - startTick);
		}
	}

	SDL_Quit();
	return 0;
}