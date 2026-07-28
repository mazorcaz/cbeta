// renderer.c

#include <cbeta/renderer.h>

#include <stdio.h>
#include <stdlib.h>
#include <cbeta/material.h>

bool cb_renderer_init(struct cb_renderer* renderer) {
	if (!cb_resource_load(&renderer->terrain, "resources/terrain.png")) {
		printf("cb_renderer_init: failed to load terrain texture\n");
		return false;
	}
	
	if (!cb_mesh_init(&renderer->mesh)) {
		printf("cb_renderer_init: failed to init mesh\n");
		return false;
	}
	
	renderer->chunks = malloc(64 * 64 * sizeof(struct cb_render_chunk));
	int i=0;
	for (int z=0; z<32; z++) {
		for (int x=0; x<32; x++,i++) {
			struct cb_render_chunk* chunk = renderer->chunks + i;
			cb_render_chunk_init(chunk);
			if (!chunk) {
				printf("cb_renderer_init: failed to init render chunk\n");
				return false;
			}
			
			chunk->x = x-16;
			chunk->z = z-16;
			
			int asdf = 0;
			if (z == 15 || z == 0) asdf = 1;
			if (x == 15 || x == 0) asdf = 2;
			
			int j=0;
			for (int z=0; z<16; z++) {
				for (int y=0; y<16; y++) {
					for (int x=0; x<16; x++,j++) {
						uint16_t material = CB_MATERIAL_DIRT;
						
						if (y == 15) {
							material = CB_MATERIAL_GRASS;
						}
						
						chunk->blocks[j] = material;
					}
				}
			}
		}
	}
	i = 0;
	for (int z=0; z<32; z++) {
		for (int x=0; x<32; x++,i++) {
			struct cb_render_chunk* chunk = renderer->chunks + i;
			
			struct cb_render_chunk* front = NULL;
			struct cb_render_chunk* back = NULL;
			struct cb_render_chunk* right = NULL;
			struct cb_render_chunk* left = NULL;
			
			if (z < 31) front = chunk + 32;
			if (z > 0) back = chunk - 32;
			if (x < 31) right = chunk + 1;
			if (x > 0) left = chunk - 1;
			
			cb_render_chunk_bake(chunk, &renderer->mesh, &renderer->terrain, front, back, right, left);
		}
	}
	
	return true;
}

void cb_renderer_free(struct cb_renderer* renderer) {
	cb_resource_free(&renderer->terrain);
	cb_mesh_free(&renderer->mesh);
	cb_render_chunk_free(renderer->chunks);
}

void cb_renderer_render(struct cb_renderer* renderer, struct cb_camera* camera) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	cb_camera_apply(camera);
			
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	
	for (int i=0; i<32*32; i++) {
		cb_render_chunk_render(renderer->chunks + i);
	}
}

bool cb_render_chunk_init(struct cb_render_chunk* chunk) {
	chunk->list = 0;
	
	chunk->blocks = malloc(4096 * 2);
	if (!chunk->blocks) {
		printf("cb_render_chunk_init: malloc failed\n");
		return false;
	}
	
	return true;
}

void cb_render_chunk_free(struct cb_render_chunk* chunk) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = 0;
	
	if (chunk->blocks) {
		free(chunk->blocks);
		chunk->blocks = NULL;
	}
}

void cb_render_chunk_bake(struct cb_render_chunk* chunk, struct cb_mesh* mesh, struct cb_resource* texture,
	struct cb_render_chunk* front, struct cb_render_chunk* back, struct cb_render_chunk* right, struct cb_render_chunk* left) {
		
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = glGenLists(1);
	
	cb_mesh_reset(mesh);
	
	int xmin = chunk->x * 16;
	int zmin = chunk->z * 16;
	int xmax = xmin + 16;
	int zmax = zmin + 16;
	
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++, i++) { // i = x + y * 16 + z * 256
				uint16_t block = chunk->blocks[i];
				struct cb_material* material = cb_materials + block;
				
				if (material->render_type == CB_RENDER_TYPE_CUBE) {
					if (z == 15 ? (!front || !cb_materials[front->blocks[i - 15 * 256]].solid) : !cb_materials[chunk->blocks[i + 256]].solid) {
						cb_cube_front(mesh, x+xmin, y, z+zmin, material->offsets[0], material->offsets[1]);
					}
					if (z == 0 ? (!back || !cb_materials[back->blocks[i + 15 * 256]].solid) : !cb_materials[chunk->blocks[i - 256]].solid) {
						cb_cube_back(mesh, x+xmin, y, z+zmin, material->offsets[2], material->offsets[3]);
					}
					if (y == 15 || !cb_materials[chunk->blocks[i + 16]].solid) {
						cb_cube_top(mesh, x+xmin, y, z+zmin, material->offsets[4], material->offsets[5]);
					}
					if (y == 0 || !cb_materials[chunk->blocks[i - 16]].solid) {
						cb_cube_bottom(mesh, x+xmin, y, z+zmin, material->offsets[6], material->offsets[7]);
					}
					if (x == 15 ? (!right || !cb_materials[right->blocks[i - 15]].solid) : !cb_materials[chunk->blocks[i + 1]].solid) {
						cb_cube_right(mesh, x+xmin, y, z+zmin, material->offsets[8], material->offsets[9]);
					}
					if (x == 0 ? (!left || !cb_materials[left->blocks[i + 15]].solid) : !cb_materials[chunk->blocks[i - 1]].solid) {
						cb_cube_left(mesh, x+xmin, y, z+zmin, material->offsets[10], material->offsets[11]);
					}
				} else if (material->render_type == CB_RENDER_TYPE_CROSS) {
					cb_cross(mesh, x+xmin, y, z+zmin, material->offsets[0], material->offsets[1]);
				}
			}
		}
	}
	
	glNewList(chunk->list, GL_COMPILE);

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

void cb_render_chunk_render(struct cb_render_chunk* chunk) {
	//glTranslatef(-chunk->x * 16.0f, -8.0f, -chunk->z * 16.0f);
	if (chunk->list) glCallList(chunk->list);
}
