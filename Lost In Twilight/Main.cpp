#include <SDL.h>
#include "GameEngine.h"

int FPS = 60;

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	GameEngine LostInTwilight;

	int desiredFrameRate = FPS;
	int frameDelay= 1000 / desiredFrameRate;
	int startTick;

	while (LostInTwilight.IsRunning()) {
		startTick = SDL_GetTicks();

		LostInTwilight.Update();

		if (SDL_GetTicks() - startTick < frameDelay) {
			SDL_Delay(frameDelay - (SDL_GetTicks() - startTick));
		}
	}

	SDL_Quit();
	return 0;
}