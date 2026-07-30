// chunk.c

#include <cbeta/chunk.h>

#include <cbeta/world.h>
#include <cbeta/geometry.h>
#include <cbeta/resource.h>
#include <cbeta/material.h>

void cb_chunk_init(struct cb_chunk* chunk, int x, int z) {
	chunk->x = x;
	chunk->z = z;
	
	for (int i=0,z=0; z<16; z++) {
		for (int y=0; y<128; y++) {
			for (int x=0; x<16; x++,i++) {
				chunk->blocks[i] = CB_MATERIAL_GRASS;
			}
		}
	}
	
	for (int i=0; i<8; i++) {
		chunk->lists[i] = 0;
		chunk->dirty[i] = true;
	}
	
	chunk->next = NULL;
}

void cb_chunk_free(struct cb_chunk* chunk) {
	for (int i=0; i<8; i++) {
		GLuint list = chunk->lists[i];
		if (list) glDeleteLists(list, 1);
	}
}

void cb_chunk_render(struct cb_chunk* chunk) {
	for (int i=0; i<8; i++) {
		cb_chunk_render_segment(chunk, i);
	}
}

void cb_chunk_render_segment(struct cb_chunk* chunk, int i) {
	GLuint list = chunk->lists[i];
	if (list) glCallList(list);
}

void cb_chunk_mesh_segment(struct cb_chunk* chunk, int i, struct cb_mesh* mesh, struct cb_world* world) {
	cb_mesh_reset(mesh);
	
	int xb = chunk->x * 16;
	int yb = i * 16;
	int zb = chunk->z * 16;
	
	int yoffset = i * 4096;
	uint16_t* blocks = chunk->blocks + yoffset;
	
	uint16_t* front = NULL;
	uint16_t* back = NULL;
	uint16_t* right = NULL;
	uint16_t* left = NULL;
	uint16_t* top = NULL;
	uint16_t* bottom = NULL;
	
	struct cb_chunk* temp;
	if (temp = cb_world_get_chunk(world, chunk->x, chunk->z + 1)) front = temp->blocks + yoffset;
	if (temp = cb_world_get_chunk(world, chunk->x, chunk->z - 1)) back = temp->blocks + yoffset;
	if (temp = cb_world_get_chunk(world, chunk->x + 1, chunk->z)) right = temp->blocks + yoffset;
	if (temp = cb_world_get_chunk(world, chunk->x - 1, chunk->z)) left = temp->blocks + yoffset;
	if (i < 7) top = blocks + 4096;
	if (i > 0) bottom = blocks - 4096;
	
	for (int i=0,z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++,i++) {
				struct cb_material* material = cb_materials + blocks[i];
				float* offsets = material->offsets;
				
				if (material->render_type == CB_RENDER_TYPE_CUBE) {
					if (z == 15 ? (!front || !cb_materials[front[i - 15 * 256]].solid) : !cb_materials[blocks[i + 256]].solid) {
						cb_cube_front(mesh, x+xb, y+yb, z+zb, offsets[0], offsets[1]);
					}
					if (z == 0 ? (!back || !cb_materials[back[i + 15 * 256]].solid) : !cb_materials[blocks[i - 256]].solid) {
						cb_cube_back(mesh, x+xb, y+yb, z+zb, offsets[2], offsets[3]);
					}
					if (y == 15 ? (!top || !cb_materials[top[i - 15 * 16]].solid) : !cb_materials[blocks[i + 16]].solid) {
						cb_cube_top(mesh, x+xb, y+yb, z+zb, offsets[4], offsets[5]);
					}
					if (y == 0 ? (!bottom || !cb_materials[bottom[i + 15 * 16]].solid) : !cb_materials[blocks[i - 16]].solid) {
						cb_cube_bottom(mesh, x+xb, y+yb, z+zb, offsets[6], offsets[7]);
					}
					if (x == 15 ? (!right || !cb_materials[right[i - 15 ]].solid) : !cb_materials[blocks[i + 1]].solid) {
						cb_cube_right(mesh, x+xb, y+yb, z+zb, offsets[8], offsets[9]);
					}
					if (x == 0 ? (!left || !cb_materials[left[i + 15]].solid) : !cb_materials[blocks[i - 1]].solid) {
						cb_cube_left(mesh, x+xb, y+yb, z+zb, offsets[10], offsets[11]);
					}
				} else if (material->render_type == CB_RENDER_TYPE_CROSS) {
					cb_cross(mesh, x+xb, y+yb, z+zb, offsets[0], offsets[1]);
				}
			}
		}
	}
}

void cb_chunk_bake_segment(struct cb_chunk* chunk, int i, struct cb_mesh* mesh, struct cb_resource* texture) {
	GLuint list = chunk->lists[i];
	if (list) glDeleteLists(list, 1);
	list = glGenLists(1);
	chunk->lists[i] = list;
	
	glNewList(list, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	
	glVertexPointer(3, GL_FLOAT, 0, mesh->pbuffer);
	glTexCoordPointer(2, GL_FLOAT, 0, mesh->tbuffer);
	
	glDrawArrays(GL_QUADS, 0, mesh->count);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glEndList();
}
