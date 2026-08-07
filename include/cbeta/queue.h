// queue.h

#ifndef CBETA_QUEUE_H
#define CBETA_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>

struct cb_queue {
	void** buffer;
	size_t size;
	
	// circular queue
	size_t front; // index of entry in front of the line
	size_t back; // index of the next free space in the back
};

bool cb_queue_init(struct cb_queue* queue);
void cb_queue_free(struct cb_queue* queue);
void* cb_queue_get(struct cb_queue* queue);
bool cb_queue_put(struct cb_queue* queue, void* thing);

#endif
