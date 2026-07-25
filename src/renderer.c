// renderer.c

#include <cbeta/renderer.h>

#include <stdio.h>
#include <stdlib.h>
#include <cbeta/material.h>

bool cb_renderer_init(struct cb_renderer* renderer) {
	renderer->terrain = malloc(sizeof(struct cb_resource));
	if (!renderer->terrain || !cb_resource_load(renderer->terrain, "resources/terrain.png")) {
		printf("cb_renderer_init: failed to load terrain texture\n");
		return false;
	}
	
	renderer->mesh = malloc(sizeof(struct cb_mesh));
	if (!renderer->mesh || !cb_mesh_init(renderer->mesh)) {
		printf("cb_renderer_init: failed to init mesh\n");
		return false;
	}
	
	renderer->chunk = malloc(sizeof(struct cb_render_chunk));
	if (!renderer->chunk) {
		printf("cb_engine_init: malloc failed\n");
		return false;
	}
	cb_render_chunk_init(renderer->chunk);
	for (int i=0; i<4096; i++) {
		renderer->chunk->blocks[i] = CB_MATERIAL_BEDROCK;
	}
	cb_render_chunk_bake(renderer->chunk, renderer->mesh, renderer->terrain);
	
	return true;
}

void cb_renderer_free(struct cb_renderer* renderer) {
	if (renderer->mesh) {
		cb_mesh_free(renderer->mesh);
		free(renderer->mesh);
		renderer->mesh = NULL;
	}
	if (renderer->terrain) {
		cb_resource_free(renderer->terrain);
		free(renderer->terrain);
		renderer->terrain = NULL;
	}
}

void cb_renderer_render(struct cb_renderer* renderer, struct cb_camera* camera) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	cb_camera_apply(camera);
			
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	
	glPushMatrix();
	glTranslatef(-8.0f, -8.0f, -20.0f);
	cb_render_chunk_render(renderer->chunk);
	glPopMatrix();
}

void cb_render_chunk_init(struct cb_render_chunk* chunk) {
	chunk->list = 0;
}

void cb_render_chunk_free(struct cb_render_chunk* chunk) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = 0;
}

void cb_render_chunk_bake(struct cb_render_chunk* chunk, struct cb_mesh* mesh, struct cb_resource* texture) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = glGenLists(1);
	
	cb_mesh_reset(mesh);
	
	int i=0;
	for (int z=0; z<16; z++) {
		for (int y=0; y<16; y++) {
			for (int x=0; x<16; x++, i++) {
				uint16_t block = chunk->blocks[i];
				struct cb_material* material = cb_materials + block;
				
				if (material->render_type == CB_RENDER_TYPE_CUBE) {
					if (z == 15 || !cb_materials[chunk->blocks[i + 256]].solid) {
						cb_cube_front(mesh, x, y, z, material->offsets[0], material->offsets[1]);
					}
					if (z == 0 || !cb_materials[chunk->blocks[i - 256]].solid) {
						cb_cube_back(mesh, x, y, z, material->offsets[2], material->offsets[3]);
					}
					if (y == 15 || !cb_materials[chunk->blocks[i + 16]].solid) {
						cb_cube_top(mesh, x, y, z, material->offsets[4], material->offsets[5]);
					}
					if (y == 0 || !cb_materials[chunk->blocks[i - 16]].solid) {
						cb_cube_bottom(mesh, x, y, z, material->offsets[6], material->offsets[7]);
					}
					if (x == 15 || !cb_materials[chunk->blocks[i + 1]].solid) {
						cb_cube_right(mesh, x, y, z, material->offsets[8], material->offsets[9]);
					}
					if (x == 0 || !cb_materials[chunk->blocks[i - 1]].solid) {
						cb_cube_left(mesh, x, y, z, material->offsets[10], material->offsets[11]);
					}
				} else if (material->render_type == CB_RENDER_TYPE_CROSS) {
					cb_cross(mesh, x, y, z, material->offsets[0], material->offsets[1]);
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
	if (chunk->list) glCallList(chunk->list);
}
