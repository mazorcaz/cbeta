// util.c

#include <cbeta/util.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* cb_strdup(const char* s) {
	size_t size = strlen(s) + 1;
	char* buf = malloc(size);
	if (!buf) {
		printf("cb_strdup: malloc failed\n");
		return NULL;
	}
	memcpy(buf, s, size);
	return buf;
}
