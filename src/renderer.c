// renderer.c

#include <cbeta/renderer.h>

void cb_render_chunk_init(struct cb_render_chunk* chunk) {
	chunk->list = 0;
}

void cb_render_chunk_bake(struct cb_render_chunk* chunk, float* vertices, float* texcoords, GLuint terrain) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = glGenLists(1);
	
	int count = 0;
	
	float* vd = vertices;
	float* td = texcoords;
	
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

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glNewList(chunk->list, GL_COMPILE);
	
	glBindTexture(GL_TEXTURE_2D, terrain);
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	
	glDrawArrays(GL_QUADS, 0, count);
	
	glEndList();
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void cb_render_chunk_render(struct cb_render_chunk* chunk) {
	if (chunk->list) glCallList(chunk->list);
}

void cb_render_chunk_free(struct cb_render_chunk* chunk) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = 0;
}
