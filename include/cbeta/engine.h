// engine.h

#ifndef CBETA_ENGINE_H
#define CBETA_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <cbeta/window.h>
#include <cbeta/camera.h>
#include <cbeta/world.h>
#include <cbeta/gui.h>

struct cb_engine {
	struct cb_window window;
	struct cb_world world;
	struct cb_gui gui;
	
	uint64_t lt;
	bool running;
	
	struct cb_camera camera;
};

bool cb_engine_init(struct cb_engine* engine);
void cb_engine_free(struct cb_engine* engine);
void cb_engine_run(struct cb_engine* engine);

#endif
