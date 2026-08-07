// subchunk.c

#include <cbeta/subchunk.h>

#include <stdio.h>
#include <cbeta/resource.h>
#include <cbeta/geometry.h>
#include <cbeta/material.h>
#include <cbeta/subchunk_cache.h>

void cb_subchunk_init(struct cb_subchunk* chunk, int x, int y, int z)
{
	chunk->x = x;
	chunk->y = y;
	chunk->z = z;
	
	chunk->list = 0;
	chunk->dirty = true;
	
	int ystart = y * 16;
	int yend = ystart + 16;
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=ystart; y<yend; y++) {
			for (int x=0; x<16; x++,i++) {
				uint16_t b = CB_MATERIAL_AIR;
				
				if (y == 127) b = CB_MATERIAL_SPRUCE_LEAVES;
				else if (y >= 120) b = CB_MATERIAL_OAK_LOG;
				else if (y > 0)  b = CB_MATERIAL_STONE;
				else if (y == 0) b = CB_MATERIAL_BEDROCK;
				
				chunk->blocks[i] = b;
			}
		}
	}
}

void cb_subchunk_free(struct cb_subchunk* chunk)
{
	if (chunk->list) glDeleteLists(chunk->list, 1);
}

void cb_subchunk_mesh(struct cb_subchunk* chunk, struct cb_mesh* mesh, struct cb_subchunk_cache* cache)
{
	cb_mesh_reset(mesh);
	
	int xb = chunk->x * 16;
	int yb = chunk->y * 16;
	int zb = chunk->z * 16;
	
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++,i++) {
				struct cb_material* material = cb_materials + chunk->blocks[i];
				float* offsets = material->offsets;
				
				if (material->render_type == CB_RENDER_TYPE_CUBE) {
					if (z == 15 ? (!cache->front || !cb_materials[cache->front->blocks[i - 15 * 256]].solid) : !cb_materials[chunk->blocks[i + 256]].solid) {
						cb_cube_front(mesh, x+xb, y+yb, z+zb, offsets[0], offsets[1]);
					}
					if (z == 0 ? (!cache->back || !cb_materials[cache->back->blocks[i + 15 * 256]].solid) : !cb_materials[chunk->blocks[i - 256]].solid) {
						cb_cube_back(mesh, x+xb, y+yb, z+zb, offsets[2], offsets[3]);
					}
					if (y == 15 ? (!cache->top || !cb_materials[cache->top->blocks[i - 15 * 16]].solid) : !cb_materials[chunk->blocks[i + 16]].solid) {
						cb_cube_top(mesh, x+xb, y+yb, z+zb, offsets[4], offsets[5]);
					}
					if (y == 0 ? (!cache->bottom || !cb_materials[cache->bottom->blocks[i + 15 * 16]].solid) : !cb_materials[chunk->blocks[i - 16]].solid) {
						cb_cube_bottom(mesh, x+xb, y+yb, z+zb, offsets[6], offsets[7]);
					}
					if (x == 15 ? (!cache->right || !cb_materials[cache->right->blocks[i - 15 ]].solid) : !cb_materials[chunk->blocks[i + 1]].solid) {
						cb_cube_right(mesh, x+xb, y+yb, z+zb, offsets[8], offsets[9]);
					}
					if (x == 0 ? (!cache->left || !cb_materials[cache->left->blocks[i + 15]].solid) : !cb_materials[chunk->blocks[i - 1]].solid) {
						cb_cube_left(mesh, x+xb, y+yb, z+zb, offsets[10], offsets[11]);
					}
				} else if (material->render_type == CB_RENDER_TYPE_CROSS) {
					cb_cross(mesh, x+xb, y+yb, z+zb, offsets[0], offsets[1]);
				}
			}
		}
	}
}

void cb_subchunk_bake(struct cb_subchunk* chunk, struct cb_mesh* mesh)
{
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = glGenLists(1);
	
	glNewList(chunk->list, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, mesh->pbuffer);
	glTexCoordPointer(2, GL_FLOAT, 0, mesh->tbuffer);
	
	glDrawArrays(GL_QUADS, 0, mesh->count);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glEndList();
}

void cb_subchunk_render(struct cb_subchunk* chunk) {
	if (chunk->list) glCallList(chunk->list);
}
