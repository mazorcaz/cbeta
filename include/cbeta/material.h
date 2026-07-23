// material.h

#ifndef CBETA_MATERIAL_H
#define CBETA_MATERIAL_H

#include <stdio.h>
#include <cbeta/geometry.h>

struct cb_material {
	const char* name;
	int offsets[12];
	float texcoords[48];
};

void cb_material_bake(struct cb_material* material);

extern struct cb_material cb_materials[];
void cb_materials_bake();

#endif
