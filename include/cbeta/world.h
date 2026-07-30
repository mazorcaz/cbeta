// world.h

#ifndef CBETA_WORLD_H
#define CBETA_WORLD_H

#include <stdint.h>
#include <stdbool.h>
#include <cbeta/camera.h>
#include <cbeta/resource.h>
#include <cbeta/geometry.h>

struct cb_chunk;
struct cb_segment;

struct cb_world {
	struct cb_mesh mesh;
	struct cb_resource terrain;
	struct cb_chunk* hashmap[4096];
};

bool cb_world_init(struct cb_world* world);
void cb_world_free(struct cb_world* world);

struct cb_chunk* cb_world_get_chunk(struct cb_world* world, int x, int z);
void cb_world_set_chunk(struct cb_world*, int x, int z, struct cb_chunk* chunk);

void cb_world_render(struct cb_world* world, struct cb_camera* camera);

#endif
