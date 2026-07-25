// geometry.h

#ifndef CBETA_GEOMETRY_H
#define CBETA_GEOMETRY_H

#include <stdbool.h>

struct cb_mesh {
	// buffers
	float* pbuffer;
	float* tbuffer;
	
	// inremented pointers
	float* p;
	float* t;
	
	// total vertices
	int count;
};

bool cb_mesh_init(struct cb_mesh* mesh);
void cb_mesh_free(struct cb_mesh* mesh);
void cb_mesh_reset(struct cb_mesh* mesh);
void cb_mesh_push(struct cb_mesh* mesh, float x, float y, float z, float u, float v);

void cb_cube_front(struct cb_mesh* mesh, float x, float y, float z, float u, float v);
void cb_cube_back(struct cb_mesh* mesh, float x, float y, float z, float u, float v);
void cb_cube_top(struct cb_mesh* mesh, float x, float y, float z, float u, float v);
void cb_cube_bottom(struct cb_mesh* mesh, float x, float y, float z, float u, float v);
void cb_cube_right(struct cb_mesh* mesh, float x, float y, float z, float u, float v);
void cb_cube_left(struct cb_mesh* mesh, float x, float y, float z, float u, float v);

void cb_cross(struct cb_mesh* mesh, float x, float y, float z, float u, float v);

void cb_fence_pole(struct cb_mesh* mesh, float x, float y, float z, float u, float v);

#endif
