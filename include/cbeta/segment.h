// segment.h

#ifndef CBETA_SEGMENT_H
#define CBETA_SEGMENT_H

#include <GL/gl.h>
#include <cbeta/geometry.h>
#include <cbeta/resource.h>

struct cb_segment {
	GLuint list;
	uint16_t blocks[4096];
	int x;
	int z;
};

void cb_segment_init(struct cb_segment* segment, int x, int z);
void cb_segment_free(struct cb_segment* segment);
void cb_segment_bake(struct cb_segment* segment, struct cb_mesh* mesh, struct cb_resource* texture,
	struct cb_segment* front, struct cb_segment* back, struct cb_segment* right, struct cb_segment* left);
void cb_segment_render(struct cb_segment* chunk);

#endif
