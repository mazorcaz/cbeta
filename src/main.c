// main.c

#include <cbeta/main.h>

#include <stdio.h>
#include <cbeta/engine.h>

int main() {
	cb_engine_init();
	cb_engine_run();
	cb_engine_free();

	return 0;
}
