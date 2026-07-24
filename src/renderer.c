// renderer.c

#include <cbeta/renderer.h>

#include <stdio.h>
#include <stdlib.h>
#include <cbeta/engine.h>
#include <cbeta/camera.h>
#include <cbeta/resource.h>
#include <cbeta/material.h>
#include <cbeta/geometry.h>

bool cb_renderer_init(struct cb_renderer* renderer) {
	renderer->scratch_vertices = malloc(4096 * 64 * 4 * 3 * sizeof(float));
	renderer->scratch_texcoords = malloc(4096 * 64 * 4 * 2 * sizeof(float));
	if (!renderer->scratch_vertices || !renderer->scratch_texcoords) {
		printf("cb_engine_init: malloc failed\n");
		return false;
	}
	
	renderer->terrain = malloc(sizeof(struct cb_resource));
	if (!renderer->terrain || !cb_resource_load(renderer->terrain, "resources/terrain.png")) {
		printf("cb_renderer_init: failed to load terrain texture\n");
		return false;
	}
	
	return true;
}

void cb_renderer_free(struct cb_renderer* renderer) {
	if (renderer->scratch_vertices) free(renderer->scratch_vertices);
	if (renderer->scratch_texcoords) free(renderer->scratch_texcoords);
	
	if (renderer->terrain) {
		cb_resource_free(renderer->terrain);
		free(renderer->terrain);
	}
}

void cb_renderer_render(struct cb_renderer* renderer) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	cb_camera_apply(cb_engine->camera);
			
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	
	glPushMatrix();
	glTranslatef(-8.0f, -8.0f, -20.0f);
	cb_render_chunk_render(cb_engine->chunk);
	glPopMatrix();
}

void cb_render_chunk_init(struct cb_render_chunk* chunk) {
	chunk->list = 0;
}

void cb_render_chunk_bake(struct cb_render_chunk* chunk) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = glGenLists(1);
	
	int count = 0;
	
	float* vd = cb_engine->renderer->scratch_vertices;
	float* td = cb_engine->renderer->scratch_texcoords;
	
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++, i++) {
				uint16_t block = chunk->blocks[i];
				struct cb_material* material = cb_materials + block;
				
				if (material->render_type == CB_RENDER_TYPE_CUBE) {
					if (z == 15 || !cb_materials[chunk->blocks[i + 256]].solid) {
						const float* vs = cb_cube_vertices + 0; 
						const float* ts = material->texcoords + 0;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (z == 0 || !cb_materials[chunk->blocks[i - 256]].solid) {
						const float* vs = cb_cube_vertices + 12; 
						const float* ts = material->texcoords + 8;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (y == 15 || !cb_materials[chunk->blocks[i + 16]].solid) {
						const float* vs = cb_cube_vertices + 24; 
						const float* ts = material->texcoords + 16;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (y == 0 || !cb_materials[chunk->blocks[i - 16]].solid) {
						const float* vs = cb_cube_vertices + 36; 
						const float* ts = material->texcoords + 24;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (x == 15 || !cb_materials[chunk->blocks[i + 1]].solid) {
						const float* vs = cb_cube_vertices + 48; 
						const float* ts = material->texcoords + 32;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (x == 0 || !cb_materials[chunk->blocks[i - 1]].solid) {
						const float* vs = cb_cube_vertices + 60;
						const float* ts = material->texcoords + 40;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
				} else if (material->render_type == CB_RENDER_TYPE_CROSS) {
					const float* vs = cb_cross_vertices; 
					const float* ts = material->texcoords;
					for (int j=0; j<16; j++) {
						*(vd++) = *(vs++) + x;
						*(vd++) = *(vs++) + y;
						*(vd++) = *(vs++) + z;
						*(td++) = *(ts++);
						*(td++) = *(ts++);
					}
					count += 16;
				} else if (material->render_type == CB_RENDER_TYPE_FENCE) {
					const float* vs = cb_fence_vertices; 
					const float* ts = material->texcoords;
					for (int j=0; j<24; j++) {
						*(vd++) = *(vs++) + x;
						*(vd++) = *(vs++) + y;
						*(vd++) = *(vs++) + z;
						*(td++) = *(ts++);
						*(td++) = *(ts++);
					}
					count += 24;
					
					if (x < 15 && chunk->blocks[i + 1] == CB_MATERIAL_FENCE) {
						vs = cb_fence_vertices + 72;
						ts = material->texcoords + 48;						
						for (int j=0; j<32; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 32;
					}
					if (x > 0 && chunk->blocks[i - 1] == CB_MATERIAL_FENCE) {
						vs = cb_fence_vertices + 168;
						ts = material->texcoords + 112;						
						for (int j=0; j<32; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 32;
					}
					if (z < 15 && chunk->blocks[i + 256] == CB_MATERIAL_FENCE) {
						vs = cb_fence_vertices + 264;
						ts = material->texcoords + 176;						
						for (int j=0; j<32; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 32;
					}
					if (z > 0 && chunk->blocks[i - 256] == CB_MATERIAL_FENCE) {
						vs = cb_fence_vertices + 360;
						ts = material->texcoords + 240;						
						for (int j=0; j<32; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 32;
					}
				}
			}
		}
	}
	
	glNewList(chunk->list, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, cb_engine->renderer->terrain->id);
	
	glVertexPointer(3, GL_FLOAT, 0, cb_engine->renderer->scratch_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, cb_engine->renderer->scratch_texcoords);
	
	glDrawArrays(GL_QUADS, 0, count);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glEndList();
}

void cb_render_chunk_render(struct cb_render_chunk* chunk) {
	if (chunk->list) glCallList(chunk->list);
}

void cb_render_chunk_free(struct cb_render_chunk* chunk) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = 0;
}
