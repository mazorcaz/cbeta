// queue.c

#include <cbeta/queue.h>

#include <stdio.h>

bool cb_queue_init(struct cb_queue* queue) {
	queue->buffer = malloc(16 * sizeof(void*));
	if (!queue->buffer) {
		printf("cb_queue_init: malloc failed\n");
		return false;
	}
	queue->size = 16;
	queue->front = 0;
	queue->back = 0;
	
	return true;
}

void cb_queue_free(struct cb_queue* queue) {
	if (queue->buffer) {
		free(queue->buffer);
		queue->buffer = NULL;
	}
	queue->size = 0;
}

void* cb_queue_get(struct cb_queue* queue) {
	if (queue->front == queue->back) return NULL;
	void* thing = queue->buffer[queue->front++];
	queue->front = queue->front % queue->size;
	return thing;
}

bool cb_queue_put(struct cb_queue* queue, void* thing) {
	if ((queue->back + 1) % queue->size == queue->front) {
		int old_farthest_slot = queue->size - 1;
		queue->size *= 2;
		void** new_buffer = malloc(queue->size * sizeof(void*));
		if (!new_buffer) {
			printf("cb_queue_put: malloc failed\n");
			return false;
		}
		for (size_t i=0; i<old_farthest_slot; i++) {
			new_buffer[i] = cb_queue_get(queue);
		}
		free(queue->buffer);
		queue->buffer = new_buffer;
		queue->front = 0;
		queue->back = old_farthest_slot;
	}
	queue->buffer[queue->back++] = thing;
	queue->back = queue->back % queue->size;
	return true;
}
