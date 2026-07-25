// geometry.c

#include <cbeta/geometry.h>

#include <stdio.h>
#include <stdlib.h>

bool cb_mesh_init(struct cb_mesh* mesh) {
	mesh->pbuffer = malloc(4096 * 64 * 4 * 3 * sizeof(float));
	mesh->tbuffer = malloc(4096 * 64 * 4 * 2 * sizeof(float));
	if (!mesh->pbuffer || !mesh->tbuffer) {
		printf("cb_mesh_init: malloc failed\n");
		return false;
	}
	
	return true;
}

void cb_mesh_free(struct cb_mesh* mesh) {
	if (mesh->pbuffer) {
		free(mesh->pbuffer);
		mesh->pbuffer = NULL;
	}
	if (mesh->tbuffer) {
		free(mesh->tbuffer);
		mesh->tbuffer = NULL;
	}
}

void cb_mesh_reset(struct cb_mesh* mesh) {
	mesh->p = mesh->pbuffer;
	mesh->t = mesh->tbuffer;
	mesh->count = 0;
}

void cb_mesh_push(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	*mesh->p++ = x;
	*mesh->p++ = y;
	*mesh->p++ = z;
	*mesh->t++ = u;
	*mesh->t++ = v;
	mesh->count++;
}

//////////
// CUBE //
//////////

void cb_cube_front(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 1.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 1.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 1.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 1.0f+z, 1.0f/16+u, 0.0f/16+v);
}

void cb_cube_back(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 0.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 0.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 0.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 0.0f+z, 1.0f/16+u, 0.0f/16+v);
}

void cb_cube_top(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 0.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 1.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 1.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 0.0f+z, 1.0f/16+u, 0.0f/16+v);
}

void cb_cube_bottom(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 1.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 0.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 0.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 1.0f+z, 1.0f/16+u, 0.0f/16+v);
}

void cb_cube_right(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 1.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 1.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 0.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 0.0f+z, 1.0f/16+u, 0.0f/16+v);
}

void cb_cube_left(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 0.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 0.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 1.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 1.0f+z, 1.0f/16+u, 0.0f/16+v);
}

///////////
// CROSS //
///////////

void cb_cross(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 0.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 0.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 1.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 1.0f+z, 1.0f/16+u, 0.0f/16+v);
	
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 1.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 1.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 0.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 0.0f+z, 1.0f/16+u, 0.0f/16+v);
	
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 1.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 1.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 0.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 0.0f+z, 1.0f/16+u, 0.0f/16+v);
	
	cb_mesh_push(mesh, 1.0f+x, 1.0f+y, 0.0f+z, 0.0f/16+u, 0.0f/16+v);
	cb_mesh_push(mesh, 1.0f+x, 0.0f+y, 0.0f+z, 0.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 0.0f+y, 1.0f+z, 1.0f/16+u, 1.0f/16+v);
	cb_mesh_push(mesh, 0.0f+x, 1.0f+y, 1.0f+z, 1.0f/16+u, 0.0f/16+v);
}

///////////
// FENCE //
///////////

void cb_fence_pole(struct cb_mesh* mesh, float x, float y, float z, float u, float v) {
	// lets not do this today
}
