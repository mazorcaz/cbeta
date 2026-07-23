// material.h

#ifndef CBETA_MATERIAL_H
#define CBETA_MATERIAL_H

#include <stdio.h>

struct cb_material {
	const char* name;
	int offsets[12];
	float texcoords[48];
};

void cb_material_init(struct cb_material* material, const char* name);
void cb_material_bake(struct cb_material* material);

#endif
