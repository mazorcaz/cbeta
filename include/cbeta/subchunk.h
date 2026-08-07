// subchunk.h

#ifndef CBETA_SUBCHUNK_H
#define CBETA_SUBCHUNK_H

#include <stdint.h>
#include <stdbool.h>
#include <GL/gl.h>

struct cb_resource;
struct cb_mesh;
struct cb_subchunk_cache;

struct cb_subchunk {
		int x;
		int y;
		int z;

		uint16_t blocks[16*16*16];

		GLuint list;
		bool dirty;
};

void cb_subchunk_init(struct cb_subchunk* chunk, int x, int y, int z);
void cb_subchunk_free(struct cb_subchunk* chunk);
void cb_subchunk_mesh(struct cb_subchunk* chunk, struct cb_mesh* mesh, struct cb_subchunk_cache* cache);
void cb_subchunk_bake(struct cb_subchunk* chunk, struct cb_mesh* mesh, struct cb_resource* texture);
void cb_subchunk_render(struct cb_subchunk* chunk);

#endif
