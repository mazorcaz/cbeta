// chunk.h

#ifndef CBETA_CHUNK_H
#define CBETA_CHUNK_H

#include <stdbool.h>
#include <stdint.h>
#include <GL/gl.h>

struct cb_world;
struct cb_mesh;
struct cb_resource;

struct cb_chunk {
	int x; // index
	int z; // index
	
	uint16_t blocks[16*128*64];
	GLuint lists[8];
	bool dirty[8];
	
	struct cb_chunk* next;
};

void cb_chunk_init(struct cb_chunk* chunk, int x, int z);
void cb_chunk_free(struct cb_chunk* chunk);
void cb_chunk_render(struct cb_chunk* chunk);
void cb_chunk_render_segment(struct cb_chunk* chunk, int i);
void cb_chunk_mesh_segment(struct cb_chunk* chunk, int i, struct cb_mesh* mesh, struct cb_world* world);
void cb_chunk_bake_segment(struct cb_chunk* chunk, int i, struct cb_mesh* mesh, struct cb_resource* texture);

#endif
