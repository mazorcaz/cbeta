// engine.h

#ifndef CBETA_ENGINE_H
#define CBETA_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

struct cb_engine {
	struct cb_window* window;
	
	uint64_t lt;
	bool running;
	
	float* vertices;
	float* texcoords;
	
	struct cb_resource* test_texture;
	struct cb_resource* terrain;
	struct cb_resource* font;
	
	struct cb_camera* camera;
	struct cb_render_chunk* chunk;
};

bool cb_engine_init();
void cb_engine_run();
void cb_engine_free();

extern struct cb_engine* cb_engine;

#endif
