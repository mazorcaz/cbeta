// renderer.h

#ifndef CBETA_RENDERER_H
#define CBETA_RENDERER_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cbeta/material.h>
#include <cbeta/geometry.h>

struct cb_render_chunk {
	GLuint list;
	uint16_t blocks[4096];
};

void cb_render_chunk_init(struct cb_render_chunk* chunk);
void cb_render_chunk_bake(struct cb_render_chunk* chunk, float* vertices, float* texcoords, GLuint terrain);
void cb_render_chunk_render(struct cb_render_chunk* chunk);
void cb_render_chunk_free(struct cb_render_chunk* chunk);

#endif
