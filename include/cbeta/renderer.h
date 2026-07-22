// renderer.h

#ifndef CBETA_RENDERER_H
#define CBETA_RENDERER_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

struct cb_render_chunk {
	GLuint list;
	uint8_t blocks[4096];
};

void cb_render_chunk_init(struct cb_render_chunk* chunk);
void cb_render_chunk_bake(struct cb_render_chunk* chunk, GLuint texture, float* vertices, float* texcoords);
void cb_render_chunk_render(struct cb_render_chunk* chunk);
void cb_render_chunk_free(struct cb_render_chunk* chunk);

extern GLfloat cb_cube_vertices[];
extern GLfloat cb_cube_colors[];
extern GLfloat cb_cube_texcoords[];

#endif
