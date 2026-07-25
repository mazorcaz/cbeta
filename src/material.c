// material.c

#include <cbeta/material.h>

#include <stdio.h>

struct cb_material cb_materials[4096];

void cb_materials_bake() {
	for (int i=0; i<4096; i++) {
		cb_materials[i] = (struct cb_material){"unknown", CB_RENDER_TYPE_CUBE, true, {14,0, 14,0, 14,0, 14,0, 14,0, 14,0}};
	}
	
	cb_materials[CB_MATERIAL_AIR]				= (struct cb_material){"air", CB_RENDER_TYPE_NONE, false, {}};
	cb_materials[CB_MATERIAL_STONE]				= (struct cb_material){"stone", CB_RENDER_TYPE_CUBE, true, {1/16.0f,0, 1/16.0f,0, 1/16.0f,0, 1/16.0f,0, 1/16.0f,0, 1/16.0f,0}};
	cb_materials[CB_MATERIAL_GRASS]				= (struct cb_material){"grass", CB_RENDER_TYPE_CUBE, true, {3/16.0f,0, 3/16.0f,0, 0,0, 2/16.0f,0, 3/16.0f,0, 3/16.0f,0}};
	cb_materials[CB_MATERIAL_DIRT]				= (struct cb_material){"dirt", CB_RENDER_TYPE_CUBE, true, {2/16.0f,0, 2/16.0f,0, 2/16.0f,0, 2/16.0f,0, 2/16.0f,0, 2/16.0f,0}};
	cb_materials[CB_MATERIAL_COBBLESTONE]		= (struct cb_material){"cobblestone", CB_RENDER_TYPE_CUBE, true, {0,1/16.0f, 0,1/16.0f, 0,1/16.0f, 0,1/16.0f, 0,1/16.0f, 0,1/16.0f}};
	cb_materials[CB_MATERIAL_PLANKS]			= (struct cb_material){"planks", CB_RENDER_TYPE_CUBE, true, {4/16.0f,0, 4/16.0f,0, 4/16.0f,0, 4/16.0f,0, 4/16.0f,0, 4/16.0f,0}};
	cb_materials[CB_MATERIAL_OAK_SAPLING]		= (struct cb_material){"oak_sapling", CB_RENDER_TYPE_CROSS, false, {15/16.0f, 0}};
	cb_materials[CB_MATERIAL_SPRUCE_SAPLING]	= (struct cb_material){"spruce_sapling", CB_RENDER_TYPE_CROSS, false, {15/16.0f, 3/16.0f}};
	cb_materials[CB_MATERIAL_BIRCH_SAPLING]		= (struct cb_material){"birch_sapling", CB_RENDER_TYPE_CROSS, false, {15/16.0f, 4/16.0f}};
	cb_materials[CB_MATERIAL_BEDROCK]			= (struct cb_material){"bedrock", CB_RENDER_TYPE_CUBE, true, {1/16.0f,1/16.0f, 1/16.0f,1/16.0f, 1/16.0f,1/16.0f, 1/16.0f,1/16.0f, 1/16.0f,1/16.0f, 1/16.0f,1/16.0f}};
	cb_materials[CB_MATERIAL_FENCE]				= (struct cb_material){"fence", CB_RENDER_TYPE_FENCE, false, {4/16.0f,0}};
}
