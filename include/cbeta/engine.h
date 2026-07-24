// engine.h

#ifndef CBETA_ENGINE_H
#define CBETA_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

struct cb_engine {
	struct cb_window* window;
	struct cb_renderer* renderer;
	struct cb_gui* gui;
	
	uint64_t lt;
	bool running;
	
	struct cb_camera* camera;
	struct cb_render_chunk* chunk;
};

bool cb_engine_init();
void cb_engine_free();
void cb_engine_run();

extern struct cb_engine* cb_engine;

#endif
