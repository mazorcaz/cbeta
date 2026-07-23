// resource.c

#include <cbeta/resource.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

bool cb_resource_load(struct cb_resource* resource, const char* file) {
	char* base_path = SDL_GetBasePath();
	if (!base_path) {
		printf("cb_resource_load: error getting base path: %s\n", SDL_GetError());
		return false;
	}
	int length = strlen(base_path);
	char* path = malloc(length + strlen(file) + 1);
	if (!path) {
		printf("cb_resource_load: malloc failed\n");
		return false;
	}
	strcpy(path, base_path);
	SDL_free(base_path);
	strcpy(path + length, file);
	SDL_Surface* raw_surface = IMG_Load(path);
	free(path);
	if (!raw_surface) {
		printf("cb_resource_load: failed to load image %s: %s\n", file, SDL_GetError());
		return false;
	}
	
	SDL_Surface* surface = SDL_ConvertSurfaceFormat(raw_surface, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(raw_surface);
	if (!surface) {
		printf("cb_resource_load: failed to convert image %s: %s\n", file, SDL_GetError());
		return false;
	}
	
	resource->width = surface->w;
	resource->height = surface->h;
	
	glGenTextures(1, &resource->id);
	glBindTexture(GL_TEXTURE_2D, resource->id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		resource->width,
		resource->height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		surface->pixels
	);
	
	SDL_FreeSurface(surface);
	return true;
}

void cb_resource_free(struct cb_resource* resource) {
	glDeleteTextures(1, &resource->id);
	resource->id = 0;
}
		
