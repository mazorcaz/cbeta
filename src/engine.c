// engine.c

#include <cbeta/engine.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <cbeta/window.h>
#include <cbeta/camera.h>
#include <cbeta/renderer.h>
#include <cbeta/resource.h>
#include <cbeta/material.h>
#include <cbeta/font.h>

struct cb_engine* cb_engine;

bool cb_engine_init() {
	
	cb_engine = malloc(sizeof(struct cb_engine));
	if (!cb_engine) {
		printf("cb_engine_init: malloc failed");
		return false;
	}
	
	// window
	cb_engine->window = malloc(sizeof(struct cb_window));
	if (!cb_engine->window || !cb_window_init(cb_engine->window)) {
		printf("cb_engine_init: failed to init window\n");
		return false;
	}
	
	// scratch
	cb_engine->vertices = malloc(4096 * 64 * 4 * 3 * sizeof(float));
	cb_engine->texcoords = malloc(4096 * 64 * 4 * 2 * sizeof(float));
	if (!cb_engine->vertices || !cb_engine->texcoords) {
		printf("cb_engine_init: malloc failed\n");
		return false;
	}
	
	// bake constants
	cb_materials_bake();
	cb_font_bake();
	
	// textures
	cb_engine->test_texture = malloc(sizeof(struct cb_resource));
	if (!cb_engine->test_texture || !cb_resource_load(cb_engine->test_texture, "resources/test.png")) {
		printf("cb_engine_init: failed to load texture\n");
		return false;
	}
	cb_engine->terrain = malloc(sizeof(struct cb_resource));
	if (!cb_engine->terrain || !cb_resource_load(cb_engine->terrain, "resources/terrain.png")) {
		printf("cb_engine_init: failed to load texture\n");
		return false;
	}
	cb_engine->font = malloc(sizeof(struct cb_resource));
	if (!cb_engine->font || !cb_resource_load(cb_engine->font, "resources/font.png")) {
		printf("cb_engine_init: failed to load texture\n");
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
			if (event.type == SDL_QUIT) {
				cb_engine->running = false;
			} else {
				cb_window_handle(cb_engine->window, &event);
				cb_camera_handle(cb_engine->camera, &event);
			}
		}

		// update
		cb_camera_update(cb_engine->camera, dt);

		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cb_camera_apply(cb_engine->camera);
				
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1f);
		
		glPushMatrix();
		glTranslatef(-8.0f, -8.0f, -20.0f);
		cb_render_chunk_render(cb_engine->chunk);
		glPopMatrix();
		
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		
		glLoadIdentity();
		glOrtho(0.0, cb_engine->window->width, cb_engine->window->height, 0.0, -1.0, 1.0);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		char str[1024];
		sprintf(str, "FPS: %i", (int)(1000.0f / (float)dt));
		
		cb_draw_string(str, 0, 0);
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		

		SDL_GL_SwapWindow(cb_engine->window->window);
	}
}

void cb_engine_free() {
	if (cb_engine) {
		free(cb_engine->vertices);
		free(cb_engine->texcoords);
		
		cb_render_chunk_free(cb_engine->chunk);
		cb_resource_free(cb_engine->test_texture);
		cb_camera_free(cb_engine->camera);
		
		cb_window_free(cb_engine->window);
		
		free(cb_engine);
	}
}
