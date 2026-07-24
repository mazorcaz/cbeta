// gui.c

#include <cbeta/gui.h>

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <cbeta/engine.h>
#include <cbeta/window.h>
#include <cbeta/font.h>
#include <cbeta/resource.h>

bool cb_gui_init(struct cb_gui* gui) {
	gui->font = malloc(sizeof(struct cb_resource));
	if (!gui->font || !cb_resource_load(gui->font, "resources/font.png")) {
		printf("cb_gui_init: failed to load font texture\n");
		return false;
	}
	
	return true;
}

void cb_gui_free(struct cb_gui* gui) {
	if (gui->font) {
		cb_resource_free(gui->font);
		free(gui->font);
	}
}

void cb_gui_render(struct cb_gui* gui) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
	glLoadIdentity();
	glOrtho(0.0, cb_engine->window->width, cb_engine->window->height, 0.0, -1.0, 1.0);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	cb_draw_string("Some Chicken Eat Chicken", 0, 0);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
