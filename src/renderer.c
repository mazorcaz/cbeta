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
				uint8_t block = chunk->blocks[i];
				
				if (block) {
					if (z == 15 || chunk->blocks[i + 256] == 0) {
						const float* vs = cb_cube_vertices + 0; 
						const float* ts = cb_materials[block].texcoords + 0;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (z == 0 || chunk->blocks[i - 256] == 0) {
						const float* vs = cb_cube_vertices + 12; 
						const float* ts = cb_materials[block].texcoords + 8;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (y == 15 || chunk->blocks[i + 16] == 0) {
						const float* vs = cb_cube_vertices + 24; 
						const float* ts = cb_materials[block].texcoords + 16;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (y == 0 || chunk->blocks[i - 16] == 0) {
						const float* vs = cb_cube_vertices + 36; 
						const float* ts = cb_materials[block].texcoords + 24;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (x == 15 || chunk->blocks[i + 1] == 0) {
						const float* vs = cb_cube_vertices + 48; 
						const float* ts = cb_materials[block].texcoords + 32;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
					if (x == 0 || chunk->blocks[i - 1] == 0) {
						const float* vs = cb_cube_vertices + 60;
						const float* ts = cb_materials[block].texcoords + 40;
						for (int j=0; j<4; j++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = *(ts++);
							*(td++) = *(ts++);
						}
						count += 4;
					}
				}
			}
		}
	}
	
	glNewList(chunk->list, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, terrain);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	
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
