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
	
	renderer->segments = malloc(32 * 32 * sizeof(struct cb_segment));
	if (!renderer->segments) {
		printf("cb_renderer_init: malloc failed\n");
		return false;
	}
	
	int i=0;
	for (int z=0; z<32; z++) {
		for (int x=0; x<32; x++,i++) {
			struct cb_segment* segment = renderer->segments + i;
			cb_segment_init(segment);
			
			segment->x = x-16;
			segment->z = z-16;
			
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
						
						segment->blocks[j] = material;
					}
				}
			}
		}
	}
	i = 0;
	for (int z=0; z<32; z++) {
		for (int x=0; x<32; x++,i++) {
			struct cb_segment* segment = renderer->segments + i;
			
			struct cb_segment* front = NULL;
			struct cb_segment* back = NULL;
			struct cb_segment* right = NULL;
			struct cb_segment* left = NULL;
			
			if (z < 31) front = segment + 32;
			if (z > 0) back = segment - 32;
			if (x < 31) right = segment + 1;
			if (x > 0) left = segment - 1;
			
			cb_segment_bake(segment, &renderer->mesh, &renderer->terrain, front, back, right, left);
			
			printf("%i%% done baking\n", i / ((32*32)/100));
		}
	}
	
	return true;
}

void cb_renderer_free(struct cb_renderer* renderer) {
	cb_resource_free(&renderer->terrain);
	cb_mesh_free(&renderer->mesh);
	cb_segment_free(renderer->segments);
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
		cb_segment_render(renderer->segments + i);
	}
}
