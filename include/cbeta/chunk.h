// chunk.h

#ifndef CBETA_CHUNK_H
#define CBETA_CHUNK_H

#include <cbeta/subchunk.h>

struct cb_world;

struct cb_chunk {
	int x; // index
	int z; // index
	
	struct cb_subchunk subchunks[8];
	
	struct cb_chunk* next;
};

void cb_chunk_init(struct cb_chunk* chunk, int x, int z);
void cb_chunk_free(struct cb_chunk* chunk);
struct cb_subchunk* cb_chunk_get_subchunk(struct cb_chunk* chunk, int y);
void cb_chunk_bake(struct cb_chunk* chunk, struct cb_world* world);
void cb_chunk_render(struct cb_chunk* chunk, struct cb_world* world);

#endif
