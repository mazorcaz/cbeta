// subchunk_cache.h

#ifndef CBETA_SUBCHUNK_CACHE_H
#define CBETA_SUBCHUNK_CACHE_H

struct cb_subchunk;
struct cb_world;

struct cb_subchunk_cache {
	struct cb_subchunk* front;
	struct cb_subchunk* back;
	struct cb_subchunk* top;
	struct cb_subchunk* bottom;
	struct cb_subchunk* right;
	struct cb_subchunk* left;
};

void cb_subchunk_cache_create(struct cb_subchunk_cache* cache, struct cb_world* world, struct cb_subchunk* chunk);

#endif
