// engine.c

#include <cbeta/engine.h>

bool cb_engine_init(struct cb_engine* engine) {
	
	////////////////////
	// INITIALIZATION //
	////////////////////
	
	// init sdl
	SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("cb_engine_init: failed to init sdl: %s\n", SDL_GetError());
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	// create window
	engine->window = SDL_CreateWindow(
		"cbeta",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	if (!engine->window) {
		printf("cb_engine_init: failed to open window: %s\n", SDL_GetError());
		return false;
	}
	
	// create opengl ctx
	engine->ctx = SDL_GL_CreateContext(engine->window);
	if (!engine->ctx) {
		printf("cb_engine_init: failed to get opengl context: %s\n", SDL_GetError());
		return false;
	}
	SDL_GL_SetSwapInterval(1);
	
	////////////////////
	// CREATE OBJECTS //
	////////////////////
	
	// scratch
	engine->vertices = malloc(4096 * 64 * 4 * 3 * sizeof(float));
	engine->texcoords = malloc(4096 * 64 * 4 * 2 * sizeof(float));
	if (!engine->vertices || !engine->texcoords) {
		printf("cb_engine_init: malloc failed\n");
		return false;
	}
	
	// materials
	cb_materials_bake();
	
	// camera
	cb_camera_init(&engine->camera);
	
	// textures
	if (!cb_resource_load(&engine->test_texture, "resources/test.png")) {
		printf("cb_engine_init: failed to load texture\n");
		return false;
	}
	if (!cb_resource_load(&engine->terrain, "resources/terrain.png")) {
		printf("cb_engine_init: failed to load texture\n");
		return false;
	}
	
	// chunk
	cb_render_chunk_init(&engine->chunk);int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++, i++) {
				if (y == 15) {
					engine->chunk.blocks[i] = CB_MATERIAL_FENCE;
				}
				else if (y == 14)
					engine->chunk.blocks[i] = CB_MATERIAL_GRASS;
				else if (y == 13)
					engine->chunk.blocks[i] = CB_MATERIAL_COBBLESTONE;
				else if (y > 9)
					engine->chunk.blocks[i] = CB_MATERIAL_DIRT;
				else if (y > 0)
					engine->chunk.blocks[i] = CB_MATERIAL_STONE;
				else
					engine->chunk.blocks[i] = CB_MATERIAL_BEDROCK;
			}
		}
	}
	engine->chunk.blocks[0] = CB_MATERIAL_OAK_SAPLING;
	cb_render_chunk_bake(&engine->chunk, engine->vertices, engine->texcoords, engine->terrain.id);
	
	/////////////////
	// LOG SUCCESS //
	/////////////////
	
	printf("cbeta initialized successfully\n");
	printf("Video Driver: %s\n", SDL_GetCurrentVideoDriver());
	printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	return true;
}

void cb_engine_run(struct cb_engine* engine) {
	
	engine->lt = SDL_GetTicks64();
	engine->focused = false;
	engine->running = true;
	
	engine->aspect = 800.0f / 600.0f;
	cb_set_perspective(100.0f, engine->aspect, 0.1f, 100.0f);
	
	SDL_Event event;
	while (engine->running) {
		// time
		uint64_t ct = SDL_GetTicks64();
		uint64_t dt = ct - engine->lt;
		engine->lt = ct;

		// events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				engine->running = false;
				
			else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				glViewport(0, 0, event.window.data1, event.window.data2);
				engine->aspect = (float)event.window.data1 / (float)event.window.data2;
				cb_set_perspective(100.0f, engine->aspect, 0.1f, 100.0f);
				
			} else if (event.type == SDL_MOUSEMOTION) {
				if (engine->focused) 
					cb_camera_handle_mouse(&engine->camera, event.motion.xrel, event.motion.yrel);
					
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					SDL_SetRelativeMouseMode(SDL_FALSE);
					engine->focused = false;
				}
				
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_SetRelativeMouseMode(SDL_TRUE);
					engine->focused = true;
				}
			}
		}

		const uint8_t* state = SDL_GetKeyboardState(NULL);

		cb_camera_handle_keys(&engine->camera, state, dt);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cb_camera_apply(&engine->camera);
				
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1f);
		
		glTranslatef(-8.0f, -8.0f, -20.0f);
		cb_render_chunk_render(&engine->chunk);

		SDL_GL_SwapWindow(engine->window);
	}
}

void cb_engine_free(struct cb_engine* engine) {
	cb_render_chunk_free(&engine->chunk);
	cb_resource_free(&engine->test_texture);
	cb_camera_free(&engine->camera);
	
	SDL_GL_DeleteContext(&engine->ctx);
	SDL_DestroyWindow(engine->window);
	SDL_Quit();
}
