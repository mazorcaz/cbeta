// window.c

#include <cbeta/window.h>

#include <GL/gl.h>
#include <cbeta/camera.h>

bool cb_window_init(struct cb_window* window) {
	// init sdl
	SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("cb_window_init: failed to init sdl: %s\n", SDL_GetError());
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	// create window
	window->window = SDL_CreateWindow(
		"cbeta",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		CB_WINDOW_DEFAULT_WIDTH, CB_WINDOW_DEFAULT_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	if (!window->window) {
		printf("cb_window_init: failed to open window: %s\n", SDL_GetError());
		return false;
	}
	
	// create opengl ctx
	window->ctx = SDL_GL_CreateContext(window->window);
	if (!window->ctx) {
		printf("cb_window_init: failed to get opengl context: %s\n", SDL_GetError());
		return false;
	}
	SDL_GL_SetSwapInterval(1);
	
	// properties
	window->focused = false;
	
	// resize
	cb_window_resize(window, CB_WINDOW_DEFAULT_WIDTH, CB_WINDOW_DEFAULT_HEIGHT);
	
	return true;
}

void cb_window_resize(struct cb_window* window, int width, int height) {
	window->width = width;
	window->height = height;
	window->aspect = (float)width / (float)height;
	
	glViewport(0, 0, window->width, window->height);
	cb_set_perspective(100.0f, window->aspect, 0.1f, 100.0f);
}

void cb_window_handle(struct cb_window* window, SDL_Event* event) {
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
		cb_window_resize(window, event->window.data1, event->window.data2);
	} else if (event->type == SDL_KEYDOWN) {
		if (event->key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			window->focused = false;
		}
	} else if (event->type == SDL_MOUSEBUTTONDOWN) {
		if (event->button.button == SDL_BUTTON_LEFT) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			window->focused = true;
		}
	}
}

void cb_window_free(struct cb_window* window) {
		SDL_GL_DeleteContext(window->ctx);
		SDL_DestroyWindow(window->window);
		SDL_Quit();
}
