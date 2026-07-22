// renderer.c

#include <cbeta/renderer.h>

void cb_render_chunk_init(struct cb_render_chunk* chunk) {
	chunk->list = 0;
}

void cb_render_chunk_bake(struct cb_render_chunk* chunk, GLuint texture, float* vertices, float* texcoords) {
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
						float* vs = cb_cube_vertices + 0; 
						float* ts = cb_cube_texcoords + 0;
						for (int i=0; i<4; i++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = (*(ts++) + block)/ 16.0f;
							*(td++) = *(ts++) / 16.0f;
						}
						count += 4;
					}
					if (z == 0 || chunk->blocks[i - 256] == 0) {
						float* vs = cb_cube_vertices + 12; 
						float* ts = cb_cube_texcoords + 8;
						for (int i=0; i<4; i++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = (*(ts++) + block) / 16.0f;
							*(td++) = *(ts++) / 16.0f;
						}
						count += 4;
					}
					if (y == 15 || chunk->blocks[i + 16] == 0) {
						float* vs = cb_cube_vertices + 24; 
						float* ts = cb_cube_texcoords + 16;
						for (int i=0; i<4; i++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = (*(ts++) + block) / 16.0f;
							*(td++) = *(ts++) / 16.0f;
						}
						count += 4;
					}
					if (y == 0 || chunk->blocks[i - 16] == 0) {
						float* vs = cb_cube_vertices + 36; 
						float* ts = cb_cube_texcoords + 24;
						for (int i=0; i<4; i++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = (*(ts++) + block) / 16.0f;
							*(td++) = *(ts++) / 16.0f;
						}
						count += 4;
					}
					if (x == 15 || chunk->blocks[i + 1] == 0) {
						float* vs = cb_cube_vertices + 48; 
						float* ts = cb_cube_texcoords + 32;
						for (int i=0; i<4; i++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = (*(ts++) + block) / 16.0f;
							*(td++) = *(ts++) / 16.0f;
						}
						count += 4;
					}
					if (x == 0 || chunk->blocks[i - 1] == 0) {
						float* vs = cb_cube_vertices + 60;
						float* ts = cb_cube_texcoords + 40;
						for (int i=0; i<4; i++) {
							*(vd++) = *(vs++) + x;
							*(vd++) = *(vs++) + y;
							*(vd++) = *(vs++) + z;
							*(td++) = (*(ts++) + block) / 16.0f;
							*(td++) = *(ts++) / 16.0f;
						}
						count += 4;
					}
				}
			}
		}
	}
	
	glNewList(chunk->list, GL_COMPILE);
				
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glTranslatef(-8.0f, -8.0f, -20.0f);
	glDrawArrays(GL_QUADS, 0, count);
	
	glPopMatrix();
	
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

// vertices
GLfloat cb_cube_vertices[] = {
	// front
	 1.0f,  1.0f,  1.0f,
	-0.0f,  1.0f,  1.0f,
	-0.0f, -0.0f,  1.0f,
	 1.0f, -0.0f,  1.0f,
	
	// back
	-0.0f, -0.0f, -0.0f,
	-0.0f,  1.0f, -0.0f,
	 1.0f,  1.0f, -0.0f,
	 1.0f, -0.0f, -0.0f,

	// top
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -0.0f,
	-0.0f,  1.0f, -0.0f,
	-0.0f,  1.0f,  1.0f,

	//bottom
	-0.0f, -0.0f, -0.0f,
	 1.0f, -0.0f, -0.0f,
	 1.0f, -0.0f,  1.0f,
	-0.0f, -0.0f,  1.0f,

	// right
	 1.0f,  1.0f,  1.0f,
	 1.0f, -0.0f,  1.0f,
	 1.0f, -0.0f, -0.0f,
	 1.0f,  1.0f, -0.0f,
	 
	// left
	-0.0f, -0.0f, -0.0f,
	-0.0f, -0.0f,  1.0f,
	-0.0f,  1.0f,  1.0f,
	-0.0f,  1.0f, -0.0f
};

// colors
GLfloat cb_cube_colors[] = {
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};

// texcoords

GLfloat cb_cube_texcoords[] = {
	// front
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	
	// back
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	
	// top
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	
	// bottom
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	
	// right
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	
	// left
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};
