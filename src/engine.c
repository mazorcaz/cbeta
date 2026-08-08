// engine.c

#include <cbeta/engine.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <cbeta/resource.h>
#include <cbeta/material.h>
#include <cbeta/font.h>

bool cb_engine_init(struct cb_engine* engine) {
	
	cb_materials_bake();
	cb_font_bake();
	
	if (!cb_window_init(&engine->window)) {
		printf("cb_engine_init: failed to init window\n");
		return false;
	}
	
	if (!cb_world_init(&engine->world)) {
		printf("cb_engine_init: failed to init world\n");
		return false;
	}
	
	if (!cb_gui_init(&engine->gui)) {
		printf("cb_engine_init: failed to init gui\n");
		return false;
	}
	
	cb_camera_init(&engine->camera);
	engine->camera.y = 130;
	
	printf("cbeta initialized successfully\n");
	printf("Video Driver: %s\n", SDL_GetCurrentVideoDriver());
	printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	return true;
}

void cb_engine_free(struct cb_engine* engine) {
	cb_window_free(&engine->window);
	cb_world_free(&engine->world);
	cb_gui_free(&engine->gui);
	cb_camera_free(&engine->camera);
}

void cb_engine_run(struct cb_engine* engine) {
	struct cb_window* window = &engine->window;
	struct cb_world* world = &engine->world;
	struct cb_gui* gui = &engine->gui;
	struct cb_camera* camera = &engine->camera;
	
	engine->lt = SDL_GetPerformanceCounter();
	engine->running = true;
	
	SDL_Event event;
	while (engine->running) {
		// time
		uint64_t ct = SDL_GetPerformanceCounter();
		uint64_t dt = ct - engine->lt;
		engine->lt = ct;

		// events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) engine->running = false;
			else {
				cb_window_handle(window, &event);
				if (window->focused) cb_camera_handle(camera, &event, world);
			}
		}

		// update
		if (window->focused) cb_camera_update(camera, dt);

		// render
		cb_world_render(world, camera);
		cb_gui_render(gui, window, dt, camera);
		
		SDL_GL_SwapWindow(window->window);
	}
}
