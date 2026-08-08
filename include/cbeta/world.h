// world.h

#ifndef CBETA_WORLD_H
#define CBETA_WORLD_H

#include <stdint.h>
#include <stdbool.h>
#include <cbeta/resource.h>
#include <cbeta/geometry.h>
#include <cbeta/queue.h>
#include <cbeta/v3.h>
#include <cbeta/loc.h>

struct cb_chunk;
struct cb_subchunk;
struct cb_camera;

struct cb_world {
	struct cb_mesh mesh;
	struct cb_resource terrain;
	struct cb_chunk* hashmap[4096];
};

bool cb_world_init(struct cb_world* world);
void cb_world_free(struct cb_world* world);

struct cb_chunk* cb_world_get_chunk(struct cb_world* world, int x, int z);
void cb_world_set_chunk(struct cb_world*, int x, int z, struct cb_chunk* chunk);

struct cb_subchunk* cb_world_get_subchunk(struct cb_world* world, int x, int y, int z);
void cb_world_dirty_subchunk(struct cb_world* world, int x, int y, int z);

uint16_t cb_world_get_block(struct cb_world* world, int x, int y, int z);
void cb_world_set_block(struct cb_world* world, int x, int y, int z, uint16_t block);

void cb_world_render(struct cb_world* world, struct cb_camera* camera);

// does the thing
void cb_world_raycast(struct cb_world* world, struct cb_v3 pos, struct cb_v3 dir, float distance,
		struct cb_loc* empty, bool* empty_found,
		struct cb_loc* solid, bool* solid_found);

#endif
