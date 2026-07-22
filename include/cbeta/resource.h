// resource.h

#ifndef CBETA_RESOURCE_H
#define CBETA_RESOURCE_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>

struct cb_resource {
	GLuint id;
	int width;
	int height;
};

bool cb_resource_load(struct cb_resource* resource, const char* file);
void cb_resource_free(struct cb_resource* resource);
	
	

#endif
