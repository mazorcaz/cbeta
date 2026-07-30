// world.c

#include <cbeta/world.h>

#include <stdlib.h>
#include <cbeta/chunk.h>

bool cb_world_init(struct cb_world* world) {
	for (int i=0; i<4096; i++) {
		world->hashmap[i] = NULL;
	}
	
	if (!cb_mesh_init(&world->mesh)) {
		printf("cb_world_init: failed to init mesh\n");
		return false;
	}
	
	if (!cb_resource_load(&world->terrain, "resources/terrain.png")) {
		printf("cb_world_init: failed to load terrain texture\n");
		return false;
	}
	
	for (int x=-2; x<2; x++) {
		for (int z=-2; z<2; z++) {
			struct cb_chunk* chunk = malloc(sizeof(struct cb_chunk));
			if (chunk == NULL) {
				printf("cb_world_init: malloc failed\n");
				return false;
			}
			cb_chunk_init(chunk, x, z);
			cb_world_set_chunk(world, x, z, chunk);
			printf("made chunk %i, %i\n", x, z);
		}
	}
	return true;
}

void cb_world_free(struct cb_world* world) {
	cb_mesh_free(&world->mesh);
	cb_resource_free(&world->terrain);
	
	for (int i=0; i<4096; i++) {
		struct cb_chunk* chunk = world->hashmap[i];
		while (chunk != NULL) {
			cb_chunk_free(chunk);
			struct cb_chunk* old = chunk;
			chunk = chunk->next;
			free(old);
		}
		world->hashmap[i] = NULL;
	}
}

struct cb_chunk* cb_world_get_chunk(struct cb_world* world, int x, int z) {
	uint32_t hash = 0x9e3779b9 ^ ((uint32_t)x * 0x1f1f1f1f) ^ ((uint32_t)z * 0x5d5d5d5d);
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash &= 4095;
	
	struct cb_chunk* chunk = world->hashmap[hash];
	printf("cb_wolrd_get_chunk: original chunk value is %p for %i, %i\n", chunk, x, z);
	if (chunk == NULL) {
		printf("cb_world_get_chunk: null hashmap value %i, %i\n", x, z);
		return NULL;
	}
	while (chunk->x != x || chunk->z != z) {
		chunk = chunk->next;
		if (chunk == NULL) return NULL;
	}
	return chunk;
}

void cb_world_set_chunk(struct cb_world* world, int x, int z, struct cb_chunk* chunk) {
	uint32_t hash = 0x9e3779b9 ^ ((uint32_t)x * 0x1f1f1f1f) ^ ((uint32_t)z * 0x5d5d5d5d);
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash &= 4095;
	
	struct cb_chunk** dest = world->hashmap + hash;
	if (*dest == NULL) printf("NULL HASHMAP[%i] ENTRY: %i, %i\n", hash, x, z);
	while (*dest != NULL) {
		if ((*dest)->x == x && (*dest)->z == z) {
			struct cb_chunk* old = *dest;
			
			if (chunk == NULL) {
				*dest = old->next;
			} else {
				*dest = chunk;
				chunk->next = old->next;
			}
			
			cb_chunk_free(old);
			free(old);
			return;
		}
		dest = &(*dest)->next;
	}
	if (chunk != NULL) {
		printf("hashmap[%i] entry: %i, %i\n", hash, x, z);
		chunk->next = NULL;
		*dest = chunk;
	}
}

void cb_world_render(struct cb_world* world, struct cb_camera* camera) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	cb_camera_apply(camera);
			
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	
	for (int i=0; i<4096; i++) {
		struct cb_chunk* chunk = world->hashmap[i];
		while (chunk != NULL) {
			cb_chunk_render(chunk, &world->mesh, &world->terrain, world);
			chunk = chunk->next;
		}
	}
}
