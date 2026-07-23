// material.h

#ifndef CBETA_MATERIAL_H
#define CBETA_MATERIAL_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <cbeta/geometry.h>

#define CB_MATERIAL_AIR ((0<<4)+0)
#define CB_MATERIAL_STONE ((1<<4)+0)
#define CB_MATERIAL_GRASS ((2<<4)+0)
#define CB_MATERIAL_DIRT ((3<<4)+0)
#define CB_MATERIAL_COBBLESTONE ((4<<4)+0)
#define CB_MATERIAL_PLANKS ((5<<4)+0)
#define CB_MATERIAL_OAK_SAPLING ((6<<4)+0)
#define CB_MATERIAL_SPRUCE_SAPLING ((6<<4)+1)
#define CB_MATERIAL_BIRCH_SAPLING ((6<<4)+2)

struct cb_material {
	const char* name;
	uint8_t render_type;
	bool solid;
	int offsets[12];
	float texcoords[48];
};

void cb_material_bake(struct cb_material* material);

extern struct cb_material cb_materials[];
void cb_materials_bake();

#endif
