// main.c

#include <cbeta/main.h>

#include <stdio.h>
#include <cbeta/engine.h>

#ifdef _WIN32
int SDL_main(int argc, char *argv[]) {
#else
int main() {
#endif
	cb_engine_init();
	cb_engine_run();
	cb_engine_free();

	return 0;
}
