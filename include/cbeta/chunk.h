// chunk.h

#ifndef CBETA_CHUNK_H
#define CBETA_CHUNK_H

#include <cbeta/segment.h>

struct cb_chunk {
	struct cb_segment segments[8];
	int x; // index
	int z; // index
	struct cb_chunk* next;
};

void cb_chunk_init(struct cb_chunk* chunk, int x, int z);
void cb_chunk_free(struct cb_chunk* chunk);
void cb_chunk_render(struct cb_chunk* chunk, struct cb_mesh* mesh, struct cb_resource* texture, struct cb_world* world);

#endif
