// subchunk_cache.c

#include <cbeta/subchunk_cache.h>

#include <cbeta/subchunk.h>
#include <cbeta/world.h>

void cb_subchunk_cache_create(struct cb_subchunk_cache* cache, struct cb_world* world, struct cb_subchunk* chunk)
{
	int x = chunk->x;
	int y = chunk->y;
	int z = chunk->z;
	
	cache->front = cb_world_get_subchunk(world, x, y, z+1);
	cache->back = cb_world_get_subchunk(world, x, y, z-1);
	cache->top = cb_world_get_subchunk(world, x, y+1, z);
	cache->bottom = cb_world_get_subchunk(world, x, y-1, z);
	cache->right = cb_world_get_subchunk(world, x+1, y, z);
	cache->left = cb_world_get_subchunk(world, x-1, y, z);
}
