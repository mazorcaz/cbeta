// world.c

#include <cbeta/world.h>

#include <stdlib.h>
#include <cbeta/chunk.h>
#include <cbeta/camera.h>
#include <cbeta/subchunk.h>

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
	
	for (int x=-4; x<4; x++) {
		for (int z=-4; z<4; z++) {
			struct cb_chunk* chunk = cb_world_get_chunk(world, x, z);
			if (chunk) cb_chunk_bake(chunk, world);
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

uint16_t cb_world_get_block(struct cb_world* world, int x, int y, int z) {
	struct cb_subchunk* chunk = cb_world_get_subchunk(world, x / 16, y / 16, z / 16);
	return chunk->blocks[(x & 15) + (y & 15) * 16 + (z & 15) * 256];
}

void cb_world_set_block(struct cb_world* world, int x, int y, int z, uint16_t block) {
	struct cb_subchunk* chunk = cb_world_get_subchunk(world, x / 16, y / 16, z / 16);
	chunk->blocks[(x & 15) + (y & 15) * 16 + (z & 15) * 256] = block;
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
	
	for (int i=0; i<4096; i++) {
		struct cb_chunk* chunk = world->hashmap[i];
		while (chunk != NULL) {
			cb_chunk_render(chunk);
			chunk = chunk->next;
		}
	}
}
