// main.c

#include <cbeta/main.h>

#include <stdio.h>
#include <stdlib.h>
#include <cbeta/engine.h>

int main() {
	struct cb_engine* engine = malloc(sizeof(struct cb_engine));
	if (!engine || !cb_engine_init(engine)) {
		printf("main: it didnt work :`(\n");
		return -1;
	}
	
	cb_engine_run(engine);
	
	cb_engine_free(engine);

	return 0;
}
