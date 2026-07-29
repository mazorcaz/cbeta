// segment.c

#include <cbeta/segment.h>

#include <cbeta/material.h>

void cb_segment_init(struct cb_segment* segment) {
	segment->list = 0;
}

void cb_segment_free(struct cb_segment* segment) {
	if (segment->list) glDeleteLists(segment->list, 1);
	segment->list = 0;
}

void cb_segment_bake(struct cb_segment* segment, struct cb_mesh* mesh, struct cb_resource* texture,
	struct cb_segment* front, struct cb_segment* back, struct cb_segment* right, struct cb_segment* left) {
		
	if (segment->list) glDeleteLists(segment->list, 1);
	segment->list = glGenLists(1);
	
	cb_mesh_reset(mesh);
	
	int xmin = segment->x * 16;
	int zmin = segment->z * 16;
	
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++, i++) { // i = x + y * 16 + z * 256
				uint16_t block = segment->blocks[i];
				struct cb_material* material = cb_materials + block;
				
				if (material->render_type == CB_RENDER_TYPE_CUBE) {
					if (z == 15 ? (!front || !cb_materials[front->blocks[i - 15 * 256]].solid) : !cb_materials[segment->blocks[i + 256]].solid) {
						cb_cube_front(mesh, x+xmin, y, z+zmin, material->offsets[0], material->offsets[1]);
					}
					if (z == 0 ? (!back || !cb_materials[back->blocks[i + 15 * 256]].solid) : !cb_materials[segment->blocks[i - 256]].solid) {
						cb_cube_back(mesh, x+xmin, y, z+zmin, material->offsets[2], material->offsets[3]);
					}
					if (y == 15 || !cb_materials[segment->blocks[i + 16]].solid) {
						cb_cube_top(mesh, x+xmin, y, z+zmin, material->offsets[4], material->offsets[5]);
					}
					if (y == 0 || !cb_materials[segment->blocks[i - 16]].solid) {
						cb_cube_bottom(mesh, x+xmin, y, z+zmin, material->offsets[6], material->offsets[7]);
					}
					if (x == 15 ? (!right || !cb_materials[right->blocks[i - 15]].solid) : !cb_materials[segment->blocks[i + 1]].solid) {
						cb_cube_right(mesh, x+xmin, y, z+zmin, material->offsets[8], material->offsets[9]);
					}
					if (x == 0 ? (!left || !cb_materials[left->blocks[i + 15]].solid) : !cb_materials[segment->blocks[i - 1]].solid) {
						cb_cube_left(mesh, x+xmin, y, z+zmin, material->offsets[10], material->offsets[11]);
					}
				} else if (material->render_type == CB_RENDER_TYPE_CROSS) {
					cb_cross(mesh, x+xmin, y, z+zmin, material->offsets[0], material->offsets[1]);
				}
			}
		}
	}
	
	glNewList(segment->list, GL_COMPILE);

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

void cb_segment_render(struct cb_segment* segment) {
	//glTranslatef(-segment->x * 16.0f, -8.0f, -segment->z * 16.0f);
	if (segment->list) glCallList(segment->list);
}
