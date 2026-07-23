// renderer.h

#ifndef CBETA_RENDERER_H
#define CBETA_RENDERER_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cbeta/material.h>

struct cb_render_chunk {
	GLuint list;
	uint8_t blocks[4096];
};

void cb_render_chunk_init(struct cb_render_chunk* chunk);
void cb_render_chunk_bake(struct cb_render_chunk* chunk, float* vertices, float* texcoords, struct cb_material* materials, GLuint terrain);
void cb_render_chunk_render(struct cb_render_chunk* chunk);
void cb_render_chunk_free(struct cb_render_chunk* chunk);

extern const float cb_cube_vertices[];
extern const float cb_cube_colors[];
extern const float cb_cube_texcoords[];

#endif
