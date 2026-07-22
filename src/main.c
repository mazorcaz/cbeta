// main.c

#include <cbeta/main.h>

int main() {

	struct cb_engine engine;
	
	if (!cb_engine_init(&engine)) {
		printf("main: failed to initialize\n");
		return -1;
	}
	cb_engine_run(&engine);
	cb_engine_free(&engine);

	return 0;
}
