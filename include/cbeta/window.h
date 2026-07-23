// window.h

#ifndef CBETA_WINDOW_H
#define CBETA_WINDOW_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define CB_WINDOW_DEFAULT_WIDTH 800
#define CB_WINDOW_DEFAULT_HEIGHT 600

struct cb_window {
	SDL_Window* window;
	SDL_GLContext ctx;
	
	int width;
	int height;
	float aspect;
	bool focused;
};

bool cb_window_init(struct cb_window* window);
void cb_window_resize(struct cb_window* window, int width, int height);
void cb_window_handle(struct cb_window* window, SDL_Event* event);
void cb_window_free(struct cb_window* window);

#endif
