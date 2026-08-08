// world.c

#include <cbeta/world.h>

#include <stdlib.h>
#include <cbeta/chunk.h>
#include <cbeta/camera.h>
#include <cbeta/subchunk.h>
#include <cbeta/v3.h>
#include <cbeta/loc.h>
#include <cbeta/material.h>

bool cb_world_init(struct cb_world* world) {
	for (int i=0; i<4096; i++) {
		world->hashmap[i] = NULL;
	}
	
	if (!cb_mesh_init(&world->mesh)) {
		printf("cb_world_init: failed to init mesh\n");
		return false;
	}
	
	if (!cb_resource_load(&world->terrain, "resources/terrain.png")) {
		printf("cb_world_init: failed to load terrain texture\n");
		return false;
	}
	
	for (int x=-4; x<4; x++) {
		for (int z=-4; z<4; z++) {
			struct cb_chunk* chunk = malloc(sizeof(struct cb_chunk));
			if (chunk == NULL) {
				printf("cb_world_init: malloc failed\n");
				return false;
			}
			cb_chunk_init(chunk, x, z);
			cb_world_set_chunk(world, x, z, chunk);
		}
	}
	
	return true;
}

void cb_world_free(struct cb_world* world) {
	cb_mesh_free(&world->mesh);
	cb_resource_free(&world->terrain);
	
	for (int i=0; i<4096; i++) {
		struct cb_chunk* chunk = world->hashmap[i];
		while (chunk != NULL) {
			cb_chunk_free(chunk);
			struct cb_chunk* old = chunk;
			chunk = chunk->next;
			free(old);
		}
		world->hashmap[i] = NULL;
	}
}

struct cb_chunk* cb_world_get_chunk(struct cb_world* world, int x, int z) {
	uint32_t hash = 0x9e3779b9 ^ ((uint32_t)x * 0x1f1f1f1f) ^ ((uint32_t)z * 0x5d5d5d5d);
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash &= 4095;
	
	struct cb_chunk* chunk = world->hashmap[hash];
	if (chunk == NULL) {
		return NULL;
	}
	while (chunk->x != x || chunk->z != z) {
		chunk = chunk->next;
		if (chunk == NULL) return NULL;
	}
	return chunk;
}

void cb_world_set_chunk(struct cb_world* world, int x, int z, struct cb_chunk* chunk) {
	uint32_t hash = 0x9e3779b9 ^ ((uint32_t)x * 0x1f1f1f1f) ^ ((uint32_t)z * 0x5d5d5d5d);
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash &= 4095;
	
	struct cb_chunk** dest = world->hashmap + hash;
	while (*dest != NULL) {
		if ((*dest)->x == x && (*dest)->z == z) {
			struct cb_chunk* old = *dest;
			
			if (chunk == NULL) {
				*dest = old->next;
			} else {
				*dest = chunk;
				chunk->next = old->next;
			}
			
			cb_chunk_free(old);
			free(old);
			return;
		}
		dest = &(*dest)->next;
	}
	if (chunk != NULL) {
		chunk->next = NULL;
		*dest = chunk;
	}
}

struct cb_subchunk* cb_world_get_subchunk(struct cb_world* world, int x, int y, int z) {
	struct cb_chunk* chunk = cb_world_get_chunk(world, x, z);
	if (!chunk) return NULL;
	return cb_chunk_get_subchunk(chunk, y);
}

void cb_world_dirty_subchunk(struct cb_world* world, int x, int y, int z) {
	struct cb_subchunk* chunk = cb_world_get_subchunk(world, x, y, z);
	if (chunk) chunk->dirty = true;
}

uint16_t cb_world_get_block(struct cb_world* world, int x, int y, int z) {
	int cx = x < 0 ? (x - 15) / 16 : x / 16;
	int cy = y < 0 ? (y - 15) / 16 : y / 16;
	int cz = z < 0 ? (z - 15) / 16 : z / 16;
	x &= 15;
	y &= 15;
	z &= 15;
	
	struct cb_subchunk* chunk = cb_world_get_subchunk(world, cx, cy, cz);
	if (!chunk) return 0;
	return chunk->blocks[x + y * 16 + z * 256];
}

void cb_world_set_block(struct cb_world* world, int x, int y, int z, uint16_t block) {
	int cx = x < 0 ? (x - 15) / 16 : x / 16;
	int cy = y < 0 ? (y - 15) / 16 : y / 16;
	int cz = z < 0 ? (z - 15) / 16 : z / 16;
	x &= 15;
	y &= 15;
	z &= 15;
	
	struct cb_subchunk* chunk = cb_world_get_subchunk(world, cx, cy, cz);
	if (!chunk) return;
	chunk->blocks[x + y * 16 + z * 256] = block;
	chunk->dirty = true;
	
	if (z == 15) cb_world_dirty_subchunk(world, cx, cy, cz + 1);
	else if (z == 0) cb_world_dirty_subchunk(world, cx, cy, cz - 1);
	if (y == 15) cb_world_dirty_subchunk(world, cx, cy + 1, cz);
	else if (y == 0) cb_world_dirty_subchunk(world, cx, cy - 1, cz);
	if (x == 15) cb_world_dirty_subchunk(world, cx + 1, cy, cz);
	else if (x == 0) cb_world_dirty_subchunk(world, cx - 1, cy, cz );
}

void cb_world_render(struct cb_world* world, struct cb_camera* camera) {
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	cb_camera_apply(camera);
			
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	
	glBindTexture(GL_TEXTURE_2D, world->terrain.id);
	
	for (int i=0; i<4096; i++) {
		struct cb_chunk* chunk = world->hashmap[i];
		while (chunk != NULL) {
			cb_chunk_render(chunk, world);
			chunk = chunk->next;
		}
	}
}

void cb_world_raycast(struct cb_world* world, struct cb_v3 pos, struct cb_v3 dir, float distance,
		struct cb_loc* empty, bool* empty_found,
		struct cb_loc* solid, bool* solid_found)
{
	// how to raycast 101
	// 1. move forward 0.1
	// 2. get the block its in
	// 3. if its solid you're done

	// make it 0.1 in the direction
	dir = cb_v3_normalize(dir);
	dir = cb_v3_muls(dir, 0.1f);

	struct cb_loc last;

	int steps = distance / 0.1f;
	for (int i=0; i<steps; i++) {
		struct cb_loc loc = cb_loc(floorf(pos.x), floorf(pos.y), floorf(pos.z));
		uint16_t block = cb_world_get_block(world, loc.x, loc.y, loc.z);

		// if solid
		if (block != CB_MATERIAL_AIR) {
			*solid_found = true;
			*solid = loc;
			if (i == 0) {
				*empty_found = false;
			} else {
				*empty_found = true;
				*empty = last;
			}
			return;
		}

		last = loc;
		pos = cb_v3_addv(pos, dir);
	}
	
	*empty_found = false;
	*solid_found = false;
}
