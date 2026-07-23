// engine.c

#include <cbeta/engine.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
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
	
	////////////////////
	// INITIALIZATION //
	////////////////////
	
	// init sdl
#ifndef _WIN32
	SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("cb_engine_init: failed to init sdl: %s\n", SDL_GetError());
		return false;
	}
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	// create window
	cb_engine->window = SDL_CreateWindow(
		"cbeta",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	if (!cb_engine->window) {
		printf("cb_engine_init: failed to open window: %s\n", SDL_GetError());
		return false;
	}
	
	// create opengl ctx
	cb_engine->ctx = SDL_GL_CreateContext(cb_engine->window);
	if (!cb_engine->ctx) {
		printf("cb_engine_init: failed to get opengl context: %s\n", SDL_GetError());
		return false;
	}
	SDL_GL_SetSwapInterval(1);
	
	////////////////////
	// CREATE OBJECTS //
	////////////////////
	
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
	
	/////////////////
	// LOG SUCCESS //
	/////////////////
	
	printf("cbeta initialized successfully\n");
	printf("Video Driver: %s\n", SDL_GetCurrentVideoDriver());
	printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	return true;
}

void cb_engine_run() {
	
	cb_engine->lt = SDL_GetTicks64();
	cb_engine->focused = false;
	cb_engine->running = true;
	
	cb_engine->width = 800;
	cb_engine->height = 600;
	cb_set_perspective(100.0f, (float)cb_engine->width / (float)cb_engine->height, 0.1f, 100.0f);
	
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
			
			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				cb_engine->width = event.window.data1;
				cb_engine->height = event.window.data2;
				glViewport(0, 0, cb_engine->width, cb_engine->height);
				cb_set_perspective(100.0f, (float)cb_engine->width / (float)cb_engine->height, 0.1f, 100.0f);
				
			} else if (event.type == SDL_MOUSEMOTION) {
				if (cb_engine->focused) 
					cb_camera_handle_mouse(cb_engine->camera, event.motion.xrel, event.motion.yrel);
					
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					SDL_SetRelativeMouseMode(SDL_FALSE);
					cb_engine->focused = false;
				}
				
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_SetRelativeMouseMode(SDL_TRUE);
					cb_engine->focused = true;
				}
			}
		}

		const uint8_t* state = SDL_GetKeyboardState(NULL);

		cb_camera_handle_keys(cb_engine->camera, state, dt);

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
		glOrtho(0.0, cb_engine->width, cb_engine->height, 0.0, -1.0, 1.0);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		char str[1024];
		sprintf(str, "fps is %i", (int)(1000.0f / (float)dt));
		
		cb_draw_string(str, 0, 0);
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		

		SDL_GL_SwapWindow(cb_engine->window);
	}
}

void cb_engine_free() {
	if (cb_engine) {
		free(cb_engine->vertices);
		free(cb_engine->texcoords);
		
		cb_render_chunk_free(cb_engine->chunk);
		cb_resource_free(cb_engine->test_texture);
		cb_camera_free(cb_engine->camera);
		
		SDL_GL_DeleteContext(cb_engine->ctx);
		SDL_DestroyWindow(cb_engine->window);
		SDL_Quit();
		
		free(cb_engine);
	}
}
