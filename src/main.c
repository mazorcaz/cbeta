// cbeta/src/main.c

#include "cbeta/main.h"

int main() {
	printf("hello world\n");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_Log("it works");

	SDL_Quit();

	return 0;
}
