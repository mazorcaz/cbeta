// material.c

#include <cbeta/material.h>

#include <stdio.h>

struct cb_material cb_materials[4096];

void cb_materials_bake() {
	for (int i=0; i<4096; i++) {
		cb_materials[i] = (struct cb_material){"unknown", CB_RENDER_TYPE_CUBE, true, {14,0, 14,0, 14,0, 14,0, 14,0, 14,0}};
	}
	
	cb_materials[CB_MATERIAL_AIR]				= (struct cb_material){"air", CB_RENDER_TYPE_NONE, false, {}};
	cb_materials[CB_MATERIAL_STONE]				= (struct cb_material){"stone", CB_RENDER_TYPE_CUBE, true, {1,0, 1,0, 1,0, 1,0, 1,0, 1,0}};
	cb_materials[CB_MATERIAL_GRASS]				= (struct cb_material){"grass", CB_RENDER_TYPE_CUBE, true, {3,0, 3,0, 0,0, 2,0, 3,0, 3,0}};
	cb_materials[CB_MATERIAL_DIRT]				= (struct cb_material){"dirt", CB_RENDER_TYPE_CUBE, true, {2,0, 2,0, 2,0, 2,0, 2,0, 2,0}};
	cb_materials[CB_MATERIAL_COBBLESTONE]		= (struct cb_material){"cobblestone", CB_RENDER_TYPE_CUBE, true, {0,1, 0,1, 0,1, 0,1, 0,1, 0,1}};
	cb_materials[CB_MATERIAL_PLANKS]			= (struct cb_material){"planks", CB_RENDER_TYPE_CUBE, true, {4,0, 4,0, 4,0, 4,0, 4,0, 4,0}};
	cb_materials[CB_MATERIAL_OAK_SAPLING]		= (struct cb_material){"oak_sapling", CB_RENDER_TYPE_CROSS, false, {15, 0}};
	cb_materials[CB_MATERIAL_SPRUCE_SAPLING]	= (struct cb_material){"spruce_sapling", CB_RENDER_TYPE_CROSS, false, {15, 3}};
	cb_materials[CB_MATERIAL_BIRCH_SAPLING]		= (struct cb_material){"birch_sapling", CB_RENDER_TYPE_CROSS, false, {15, 4}};
	cb_materials[CB_MATERIAL_BEDROCK]			= (struct cb_material){"bedrock", CB_RENDER_TYPE_CUBE, true, {1,1, 1,1, 1,1, 1,1, 1,1, 1,1}};
	cb_materials[CB_MATERIAL_SAND]				= (struct cb_material){"sand", CB_RENDER_TYPE_CUBE, true, {2,1, 2,1, 2,1, 2,1, 2,1, 2,1}};
	cb_materials[CB_MATERIAL_GRAVEL]			= (struct cb_material){"gravel", CB_RENDER_TYPE_CUBE, true, {3,1, 3,1, 3,1, 3,1, 3,1, 3,1}};
	cb_materials[CB_MATERIAL_GOLD_ORE]			= (struct cb_material){"gold_ore", CB_RENDER_TYPE_CUBE, true, {0,2, 0,2, 0,2, 0,2, 0,2, 0,2}};
	cb_materials[CB_MATERIAL_IRON_ORE]			= (struct cb_material){"iron_ore", CB_RENDER_TYPE_CUBE, true, {1,2, 1,2, 1,2, 1,2, 1,2, 1,2}};
	cb_materials[CB_MATERIAL_COAL_ORE]			= (struct cb_material){"coal_ore", CB_RENDER_TYPE_CUBE, true, {2,2, 2,2, 2,2, 2,2, 2,2, 2,2}};
	cb_materials[CB_MATERIAL_OAK_LOG]			= (struct cb_material){"oak_log", CB_RENDER_TYPE_CUBE, true, {4,1, 4,1, 5,1, 5,1, 4,1, 4,1}};
	cb_materials[CB_MATERIAL_SPRUCE_LOG]		= (struct cb_material){"spruce_log", CB_RENDER_TYPE_CUBE, true, {4,7, 4,7, 5,1, 5,1, 4,7, 4,7}};
	cb_materials[CB_MATERIAL_BIRCH_LOG]			= (struct cb_material){"birch_log", CB_RENDER_TYPE_CUBE, true, {5,7, 5,7, 5,1, 5,1, 5,7, 5,7}};
	cb_materials[CB_MATERIAL_OAK_LEAVES]		= (struct cb_material){"oak_leaves", CB_RENDER_TYPE_CUBE, false, {4,3, 4,3, 4,3, 4,3, 4,3, 4,3}};
	cb_materials[CB_MATERIAL_SPRUCE_LEAVES]		= (struct cb_material){"spruce_leaves", CB_RENDER_TYPE_CUBE, false, {4,8, 4,8, 4,8, 4,8, 4,8, 4,8}};
	cb_materials[CB_MATERIAL_BIRCH_LEAVES]		= (struct cb_material){"birch_leaves", CB_RENDER_TYPE_CUBE, false, {4,3, 4,3, 4,3, 4,3, 4,3, 4,3}};
	cb_materials[CB_MATERIAL_FENCE]				= (struct cb_material){"fence", CB_RENDER_TYPE_FENCE, false, {4,0}};
	
	for (int i=0; i<4096; i++) {
		for (int j=0; j<12; j++) {
			float v = cb_materials[i].offsets[j];
			cb_materials[i].offsets[j] /= 16.0f;
		}
	}
}
