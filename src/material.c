// material.c

#include <cbeta/material.h>

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

struct cb_material cb_materials[256] = {
	{"air", {0,0, 0,0, 0,0, 0,0, 0,0, 0,0}, {}},
	{"stone", {1,0, 1,0, 1,0, 1,0, 1,0, 1,0}, {}},
	{"dirt", {2,0, 2,0, 2,0, 2,0, 2,0, 2,0}, {}},
	{"grass", {3,0, 3,0, 0,0, 2,0, 3,0, 3,0}, {}}
};

void cb_materials_bake() {
	for (int i=0; i<256; i++) {
		cb_material_bake(cb_materials + i);
	}
}
