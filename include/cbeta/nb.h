// nb.h

#ifndef CBETA_NB_H
#define CBETA_NB_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct cb_nb {
	int fd;
};

bool cb_nb_init(struct cb_nb* nb, const char* ip, uint16_t port);
void cb_nb_free(struct cb_nb* nb);

bool cb_nb_read(struct cb_nb* nb, void* dst, size_t size);
bool cb_nb_write(struct cb_nb* nb, void* src, size_t size);

bool cb_nb_rbyte(struct cb_nb* nb, uint8_t* v);
bool cb_nb_wbyte(struct cb_nb* nb, uint8_t v);

bool cb_nb_rshort(struct cb_nb* nb, uint16_t* v);
bool cb_nb_wshort(struct cb_nb* nb, uint16_t v);

bool cb_nb_rint(struct cb_nb* nb, uint32_t* v);
bool cb_nb_wint(struct cb_nb* nb, uint32_t v);

bool cb_nb_rlong(struct cb_nb* nb, uint64_t* v);
bool cb_nb_wlong(struct cb_nb* nb, uint64_t v);

bool cb_nb_rstring16(struct cb_nb* nb, char** v);
bool cb_nb_wstring16(struct cb_nb* nb, const char* v);

#endif
