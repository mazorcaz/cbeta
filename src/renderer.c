// renderer.c

#include <cbeta/renderer.h>

void cb_cube_init(struct cb_cube* cube, float x, float y, float z) {
	cube->x = x;
	cube->y = y;
	cube->z = z;
}

void cb_cube_render(struct cb_cube* cube, struct cb_camera* camera) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, cube->texture);
	glVertexPointer(3, GL_FLOAT, 0, cube->vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, cube->texcoords);

	glMatrixMode(GL_MODELVIEW);
	cb_camera_apply(camera);
	
	glTranslatef(cube->x, cube->y, cube->z);

	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void cb_cube_free(struct cb_cube* cube) {
	// do nothing
}

void cb_render_chunk_init(struct cb_render_chunk* chunk) {
	chunk->list = 0;
}

void cb_render_chunk_bake(struct cb_render_chunk* chunk, GLuint texture, struct cb_camera* camera) {
	if (chunk->list) glDeleteLists(chunk->list, 1);
	chunk->list = glGenLists(1);
	
	glNewList(chunk->list, GL_COMPILE);
	
	struct cb_cube cube;
	cb_cube_init(&cube, 0.0f, 0.0f, 0.0f);
	cube.vertices = cb_cube_vertices;
	cube.texcoords = cb_cube_texcoords;
	cube.texture = texture;
	
	for (int x=0; x<16; x++) {
		for (int y=0; y<16; y++) {
			for (int z=0; z<16; z++) {
				int i = x + (y * 16) + (z * 256);
				
				cube.x = (float)x - 8.0f;
				cube.y = (float)y - 8.0f;
				cube.z = (float)z - 20.0f;
				
				cb_cube_render(&cube, camera);
			}
		}
	}
	
	cb_cube_free(&cube);
	
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
