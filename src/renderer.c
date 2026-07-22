// renderer.c

#include <cbeta/renderer.h>

void cb_render_chunk_init(struct cb_render_chunk* chunk) {
	chunk->list = 0;
}

void cb_render_chunk_bake(struct cb_render_chunk* chunk, GLuint texture) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = glGenLists(1);
	
	glNewList(chunk->list, GL_COMPILE);
				
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	for (int x=0; x<16; x++) {
		for (int y=0; y<16; y++) {
			for (int z=0; z<16; z++) {
				int i = x + (y * 16) + (z * 256);

				glMatrixMode(GL_MODELVIEW);	
				glPushMatrix();

				glBindTexture(GL_TEXTURE_2D, texture);
				glVertexPointer(3, GL_FLOAT, 0, cb_cube_vertices);
				glTexCoordPointer(2, GL_FLOAT, 0, cb_cube_texcoords);
				glTranslatef((float)x - 8.0f, (float)y - 8.0f, (float)z - 20.0f);
				glDrawArrays(GL_QUADS, 0, 24);
				
				glPopMatrix();

			}
		}
	}
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

	// left
	-0.0f, -0.0f, -0.0f,
	-0.0f, -0.0f,  1.0f,
	-0.0f,  1.0f,  1.0f,
	-0.0f,  1.0f, -0.0f,

	// right
	 1.0f,  1.0f,  1.0f,
	 1.0f, -0.0f,  1.0f,
	 1.0f, -0.0f, -0.0f,
	 1.0f,  1.0f, -0.0f
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

	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
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
	
	// left
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	
	// right
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};
