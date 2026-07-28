// gui.c

#include <cbeta/gui.h>

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <cbeta/font.h>

bool cb_gui_init(struct cb_gui* gui) {
	if (!cb_resource_load(&gui->font, "resources/font.png")) {
		printf("cb_gui_init: failed to load font texture\n");
		return false;
	}
	
	return true;
}

void cb_gui_free(struct cb_gui* gui) {
	cb_resource_free(&gui->font);
}

void cb_gui_render(struct cb_gui* gui, struct cb_window* window, uint64_t dt) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
	glLoadIdentity();
	glOrtho(0.0, window->width, window->height, 0.0, -1.0, 1.0);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	char string[1024];
	sprintf(string, "fps: %i", (int)((float)SDL_GetPerformanceFrequency() / (float)dt));
	cb_draw_string(string, 0, 0, &gui->font);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
