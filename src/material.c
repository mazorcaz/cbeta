// material.c

#include <cbeta/material.h>
#include <cbeta/renderer.h>

void cb_material_init(struct cb_material* material, const char* name) {
	material->name = name;
}

void cb_material_bake(struct cb_material* material) {
	for (int i=0; i<6; i++) {
		material->texcoords[i*8+0] = (cb_cube_texcoords[i*8+0] + material->offsets[i*2+0]) / 16.0f;
		material->texcoords[i*8+1] = (cb_cube_texcoords[i*8+1] + material->offsets[i*2+1]) / 16.0f;
		material->texcoords[i*8+2] = (cb_cube_texcoords[i*8+2] + material->offsets[i*2+0]) / 16.0f;
		material->texcoords[i*8+3] = (cb_cube_texcoords[i*8+3] + material->offsets[i*2+1]) / 16.0f;
		material->texcoords[i*8+4] = (cb_cube_texcoords[i*8+4] + material->offsets[i*2+0]) / 16.0f;
		material->texcoords[i*8+5] = (cb_cube_texcoords[i*8+5] + material->offsets[i*2+1]) / 16.0f;
		material->texcoords[i*8+6] = (cb_cube_texcoords[i*8+6] + material->offsets[i*2+0]) / 16.0f;
		material->texcoords[i*8+7] = (cb_cube_texcoords[i*8+7] + material->offsets[i*2+1]) / 16.0f;
	}
}
