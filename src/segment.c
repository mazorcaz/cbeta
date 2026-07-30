// segment.c

#include <cbeta/segment.h>

#include <stdint.h>
#include <cbeta/material.h>
#include <cbeta/chunk.h>

void cb_segment_init(struct cb_segment* segment, int x, int y, int z) {
	segment->dirty = true;
	segment->list = 0;
	segment->x = x;
	segment->y = y;
	segment->z = z;
	
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++, i++) {
				uint16_t material = CB_MATERIAL_DIRT;
				
				if (y == 15) material = CB_MATERIAL_GRASS;
				if (y < 10) material = CB_MATERIAL_STONE;
				if (y == 0) material = CB_MATERIAL_BEDROCK;
				
				segment->blocks[i] = material;
			}
		}
	}
}

void cb_segment_free(struct cb_segment* segment) {
	if (segment->list) glDeleteLists(segment->list, 1);
	segment->list = 0;
}

void cb_segment_bake(struct cb_segment* segment, struct cb_mesh* mesh, struct cb_resource* texture, struct cb_world* world) {
		
	if (segment->list) glDeleteLists(segment->list, 1);
	segment->list = glGenLists(1);
	
	cb_mesh_reset(mesh);
	
	int xb = segment->x * 16;
	int yb = segment->y * 16;
	int zb = segment->z * 16;
	
	struct cb_segment* front = NULL;
	struct cb_segment* back = NULL;
	struct cb_segment* top = NULL;
	struct cb_segment* bottom = NULL;
	struct cb_segment* right = NULL;
	struct cb_segment* left = NULL;
	
	struct cb_chunk* chunk;
	if (chunk = cb_world_get_chunk(world, segment->x, segment->z + 1)) front = chunk->segments + segment->y;
	if (chunk = cb_world_get_chunk(world, segment->x, segment->z - 1)) back = chunk->segments + segment->y;
	if (chunk = cb_world_get_chunk(world, segment->x + 1, segment->z)) right = chunk->segments + segment->y;
	if (chunk = cb_world_get_chunk(world, segment->x - 1, segment->z)) left = chunk->segments + segment->y;
	if (segment->y < 7) top = segment + 1;
	if (segment->y > 0) bottom = segment - 1;
	
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++, i++) { // i = x + y * 16 + z * 256
				uint16_t block = segment->blocks[i];
				struct cb_material* material = cb_materials + block;
				
				if (material->render_type == CB_RENDER_TYPE_CUBE) {
					if (z == 15 ? (!front || !cb_materials[front->blocks[i - 15 * 256]].solid) : !cb_materials[segment->blocks[i + 256]].solid) {
						cb_cube_front(mesh, x+xb, y+yb, z+zb, material->offsets[0], material->offsets[1]);
					}
					if (z == 0 ? (!back || !cb_materials[back->blocks[i + 15 * 256]].solid) : !cb_materials[segment->blocks[i - 256]].solid) {
						cb_cube_back(mesh, x+xb, y+yb, z+zb, material->offsets[2], material->offsets[3]);
					}
					if (y == 15 ? (!top || !cb_materials[top->blocks[i - 15 * 16]].solid) : !cb_materials[segment->blocks[i + 16]].solid) {
						cb_cube_top(mesh, x+xb, y+yb, z+zb, material->offsets[4], material->offsets[5]);
					}
					if (y == 0 ? (!bottom || !cb_materials[bottom->blocks[i + 15 * 16]].solid) : !cb_materials[segment->blocks[i - 16]].solid) {
						cb_cube_bottom(mesh, x+xb, y+yb, z+zb, material->offsets[6], material->offsets[7]);
					}
					if (x == 15 ? (!right || !cb_materials[right->blocks[i - 15]].solid) : !cb_materials[segment->blocks[i + 1]].solid) {
						cb_cube_right(mesh, x+xb, y+yb, z+zb, material->offsets[8], material->offsets[9]);
					}
					if (x == 0 ? (!left || !cb_materials[left->blocks[i + 15]].solid) : !cb_materials[segment->blocks[i - 1]].solid) {
						cb_cube_left(mesh, x+xb, y+yb, z+zb, material->offsets[10], material->offsets[11]);
					}
				} else if (material->render_type == CB_RENDER_TYPE_CROSS) {
					cb_cross(mesh, x+xb, y+yb, z+zb, material->offsets[0], material->offsets[1]);
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
	
	segment->dirty = false;
}

void cb_segment_render(struct cb_segment* segment, struct cb_mesh* mesh, struct cb_resource* texture, struct cb_world* world) {
	if (segment->dirty) {
		cb_segment_bake(segment, mesh, texture, world);
	}
	glCallList(segment->list);
}
