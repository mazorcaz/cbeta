// engine.h

#ifndef CBETA_ENGINE_H
#define CBETA_ENGINE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cbeta/camera.h>
#include <cbeta/renderer.h>
#include <cbeta/resource.h>

struct cb_engine {
	SDL_Window* window;
	SDL_GLContext ctx;
	
	uint64_t lt;
	bool running;
	bool focused;
	float aspect;
	
	struct cb_camera camera;
	struct cb_resource test_texture;
	struct cb_render_chunk chunk;
};

bool cb_engine_init(struct cb_engine* engine);
void cb_engine_run(struct cb_engine* engine);
void cb_engine_free(struct cb_engine* engine);

#endif
