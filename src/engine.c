// engine.c

#include <cbeta/engine.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <cbeta/window.h>
#include <cbeta/camera.h>
#include <cbeta/renderer.h>
#include <cbeta/gui.h>
#include <cbeta/resource.h>
#include <cbeta/material.h>
#include <cbeta/font.h>

bool cb_engine_init(struct cb_engine* engine) {
	
	// bake constants
	cb_materials_bake();
	cb_font_bake();
	
	// window
	engine->window = malloc(sizeof(struct cb_window));
	if (!engine->window || !cb_window_init(engine->window)) {
		printf("cb_engine_init: failed to init window\n");
		return false;
	}
	
	// renderer
	engine->renderer = malloc(sizeof(struct cb_renderer));
	if (!engine->renderer || !cb_renderer_init(engine->renderer)) {
		printf("cb_engine_init: failed to init renderer\n");
		return false;
	}
	
	// gui
	engine->gui = malloc(sizeof(struct cb_gui));
	if (!engine->gui || !cb_gui_init(engine->gui)) {
		printf("cb_engine_init: failed to init gui\n");
		return false;
	}
	
	// camera
	engine->camera = malloc(sizeof(struct cb_camera));
	if (!engine->camera) {
		printf("cb_engine_init: malloc failed\n");
		return false;
	}
	cb_camera_init(engine->camera);
	
	printf("cbeta initialized successfully\n");
	printf("Video Driver: %s\n", SDL_GetCurrentVideoDriver());
	printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	return true;
}

void cb_engine_free(struct cb_engine* engine) {
	if (engine->window) {
		cb_window_free(engine->window);
		free(engine->window);
	}
	if (engine->renderer) {
		cb_renderer_free(engine->renderer);
		free(engine->renderer);
	}
	if (engine->gui) {
		cb_gui_free(engine->gui);
		free(engine->gui);
	}
	if (engine->camera) {
		cb_camera_free(engine->camera);
		free(engine->camera);
	}
}

void cb_engine_run(struct cb_engine* engine) {
	
	engine->lt = SDL_GetTicks64();
	engine->running = true;
	
	SDL_Event event;
	while (engine->running) {
		// time
		uint64_t ct = SDL_GetTicks64();
		uint64_t dt = ct - engine->lt;
		engine->lt = ct;

		// events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) engine->running = false;
			else {
				cb_window_handle(engine->window, &event);
				if (engine->window->focused) cb_camera_handle(engine->camera, &event);
			}
		}

		// update
		if (engine->window->focused) cb_camera_update(engine->camera, dt);

		// render
		cb_renderer_render(engine->renderer, engine->camera);
		cb_gui_render(engine->gui, engine->window);
		
		SDL_GL_SwapWindow(engine->window->window);
	}
}
