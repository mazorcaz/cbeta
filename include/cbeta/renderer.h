// renderer.h

#ifndef CBETA_RENDERER_H
#define CBETA_RENDERER_H

#include <stdint.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <cbeta/camera.h>
#include <cbeta/resource.h>
#include <cbeta/geometry.h>

struct cb_render_chunk {
	GLuint list;
	uint16_t blocks[4096];
	int x;
	int z;
};

void cb_render_chunk_init(struct cb_render_chunk* chunk);
void cb_render_chunk_free(struct cb_render_chunk* chunk);
void cb_render_chunk_bake(struct cb_render_chunk* chunk, struct cb_mesh* mesh, struct cb_resource* texture,
	struct cb_render_chunk* front, struct cb_render_chunk* back, struct cb_render_chunk* right, struct cb_render_chunk* left);
void cb_render_chunk_render(struct cb_render_chunk* chunk);

struct cb_renderer {
	struct cb_resource terrain;
	struct cb_mesh mesh;
	struct cb_render_chunk* chunks;
};

bool cb_renderer_init(struct cb_renderer* renderer);
void cb_renderer_free(struct cb_renderer* renderer);
void cb_renderer_render(struct cb_renderer* renderer, struct cb_camera* camera);

#endif
