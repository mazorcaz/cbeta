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

struct cb_engine* cb_engine;

bool cb_engine_init() {
	
	cb_engine = malloc(sizeof(struct cb_engine));
	if (!cb_engine) {
		printf("cb_engine_init: malloc failed\n");
		return false;
	}
	
	// bake constants
	cb_materials_bake();
	cb_font_bake();
	
	// window
	cb_engine->window = malloc(sizeof(struct cb_window));
	if (!cb_engine->window || !cb_window_init(cb_engine->window)) {
		printf("cb_engine_init: failed to init window\n");
		return false;
	}
	
	// renderer
	cb_engine->renderer = malloc(sizeof(struct cb_renderer));
	if (!cb_engine->renderer || !cb_renderer_init(cb_engine->renderer)) {
		printf("cb_engine_init: failed to init renderer\n");
		return false;
	}
	
	// gui
	cb_engine->gui = malloc(sizeof(struct cb_gui));
	if (!cb_engine->gui || !cb_gui_init(cb_engine->gui)) {
		printf("cb_engine_init: failed to init gui\n");
		return false;
	}
	
	// camera
	cb_engine->camera = malloc(sizeof(struct cb_camera));
	if (!cb_engine->camera) {
		printf("cb_engine_init: malloc failed\n");
		return false;
	}
	cb_camera_init(cb_engine->camera);
	
	// chunk
	cb_engine->chunk = malloc(sizeof(struct cb_render_chunk));
	if (!cb_engine->chunk) {
		printf("cb_engine_init: malloc failed\n");
		return false;
	}
	cb_render_chunk_init(cb_engine->chunk);
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++, i++) {
				uint16_t block;
				
				if (y == 15) {
					if (i % 5 == 0) block = CB_MATERIAL_AIR;
					else block = CB_MATERIAL_FENCE;
				}
				else if (y == 14)
					block = CB_MATERIAL_GRASS;
				else if (y == 13)
					block = CB_MATERIAL_COBBLESTONE;
				else if (y > 9)
					block = CB_MATERIAL_DIRT;
				else if (y > 0)
					block = CB_MATERIAL_STONE;
				else
					block = CB_MATERIAL_BEDROCK;
					
				cb_engine->chunk->blocks[i] = block;
			}
		}
	}
	cb_engine->chunk->blocks[0] = CB_MATERIAL_OAK_SAPLING;
	cb_render_chunk_bake(cb_engine->chunk);
	
	printf("cbeta initialized successfully\n");
	printf("Video Driver: %s\n", SDL_GetCurrentVideoDriver());
	printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	return true;
}

void cb_engine_free() {
	if (cb_engine) {
		if (cb_engine->window) {
			cb_window_free(cb_engine->window);
			free(cb_engine->window);
		}
		if (cb_engine->renderer) {
			cb_renderer_free(cb_engine->renderer);
			free(cb_engine->renderer);
		}
		if (cb_engine->gui) {
			cb_gui_free(cb_engine->gui);
			free(cb_engine->gui);
		}
		if (cb_engine->camera) {
			cb_camera_free(cb_engine->camera);
			free(cb_engine->camera);
		}
		
		cb_render_chunk_free(cb_engine->chunk);
		
		free(cb_engine);
	}
}

void cb_engine_run() {
	
	cb_engine->lt = SDL_GetTicks64();
	cb_engine->running = true;
	
	SDL_Event event;
	while (cb_engine->running) {
		// time
		uint64_t ct = SDL_GetTicks64();
		uint64_t dt = ct - cb_engine->lt;
		cb_engine->lt = ct;

		// events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) cb_engine->running = false;
			else {
				cb_window_handle(cb_engine->window, &event);
				cb_camera_handle(cb_engine->camera, &event);
			}
		}

		// update
		cb_camera_update(cb_engine->camera, dt);

		// render
		cb_renderer_render(cb_engine->renderer);
		cb_gui_render(cb_engine->gui);
		
		SDL_GL_SwapWindow(cb_engine->window->window);
	}
}
