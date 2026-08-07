// chunk.c

#include <cbeta/chunk.h>

#include <stddef.h>
#include <stdio.h>
#include <cbeta/world.h>
#include <cbeta/geometry.h>
#include <cbeta/resource.h>
#include <cbeta/subchunk_cache.h>

void cb_chunk_init(struct cb_chunk* chunk, int x, int z) {
	chunk->x = x;
	chunk->z = z;
	
	for (int i=0; i<8; i++) {
		cb_subchunk_init(chunk->subchunks + i, x, i, z);
	}
	
	chunk->next = NULL;
}

void cb_chunk_free(struct cb_chunk* chunk) {
	for (int i=0; i<8; i++) {
		cb_subchunk_free(chunk->subchunks + i);
	}
}

struct cb_subchunk* cb_chunk_get_subchunk(struct cb_chunk* chunk, int y) {
	if (y >= 0 && y <= 7) return chunk->subchunks + y;
	return NULL;
}

void cb_chunk_render(struct cb_chunk* chunk, struct cb_world* world) {
	for (int i=0; i<8; i++) {
		cb_subchunk_render(chunk->subchunks + i, world);
	}
}

void cb_chunk_bake(struct cb_chunk* chunk, struct cb_world* world)
{
	int x = chunk->x;
	int z = chunk->z;
	for (int y=0; y<8; y++) {
		struct cb_subchunk* subchunk = cb_chunk_get_subchunk(chunk, y);
		struct cb_subchunk_cache cache;
		cache.front = cb_world_get_subchunk(world, x, y, z + 1);
		cache.back = cb_world_get_subchunk(world, x, y, z - 1);
		cache.top = cb_chunk_get_subchunk(chunk, y + 1);
		cache.bottom = cb_chunk_get_subchunk(chunk, y - 1);
		cache.right = cb_world_get_subchunk(world, x + 1, y, z);
		cache.left = cb_world_get_subchunk(world, x - 1, y, z);
		
		cb_subchunk_mesh(subchunk, &world->mesh, &cache);
		cb_subchunk_bake(subchunk, &world->mesh);
	}
}
