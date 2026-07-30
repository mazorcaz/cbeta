// chunk.c

#include <cbeta/chunk.h>

void cb_chunk_init(struct cb_chunk* chunk, int x, int z) {
	chunk->x = x;
	chunk->z = z;
	chunk->next = NULL;
	for (int i=0; i<8; i++) {
		cb_segment_init(chunk->segments + i, x, i, z);
	}
}

void cb_chunk_free(struct cb_chunk* chunk) {
	for (int i=0; i<8; i++) {
		cb_segment_free(chunk->segments + i);
	}
}

void cb_chunk_render(struct cb_chunk* chunk, struct cb_mesh* mesh, struct cb_resource* texture, struct cb_world* world) {
	for (int i=0; i<8; i++) {
		cb_segment_render(chunk->segments + i, mesh, texture, world);
	}
}
