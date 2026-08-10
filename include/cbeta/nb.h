// nb.h

#ifndef CBETA_NB_H
#define CBETA_NB_H

#include <stdint.h>
#include <stddef.h>

struct cb_nb {
	int fd;
};

bool cb_nb_init(struct cb_nb* nb, const char* ip, uint16_t port);
void cb_nb_free(struct cb_nb* nb);

bool cb_nb_read(struct cb_nb* nb, void* dst, size_t size);
bool cb_nb_write(struct cb_nb* nb, void* src, size_t size);

bool cb_nb_rbyte(struct cb_nb* nb, uint8_t* v);
bool cb_nb_wbyte(struct cb_nb* nb, uint8_t v);

#endif
