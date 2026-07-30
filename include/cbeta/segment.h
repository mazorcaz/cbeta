// segment.h

#ifndef CBETA_SEGMENT_H
#define CBETA_SEGMENT_H

#include <stdbool.h>
#include <GL/gl.h>
#include <cbeta/world.h>
#include <cbeta/geometry.h>
#include <cbeta/resource.h>

struct cb_segment {
	bool dirty;
	GLuint list;
	uint16_t blocks[4096];
	int x;
	int y;
	int z;
};

void cb_segment_init(struct cb_segment* segment, int x, int y, int z);
void cb_segment_free(struct cb_segment* segment);
void cb_segment_bake(struct cb_segment* segment, struct cb_mesh* mesh, struct cb_resource* texture, struct cb_world* world);
void cb_segment_render(struct cb_segment* segment, struct cb_mesh* mesh, struct cb_resource* texture, struct cb_world* world);

#endif
