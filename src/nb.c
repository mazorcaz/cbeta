// nb.c

#include <cbeta/nb.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

bool cb_nb_init(struct cb_nb* nb, const char* ip, uint16_t port) {
	nb->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (nb->fd < 0) {
		printf("cb_nb_init: socket failed\n");
		return false;
	}
	
	struct sockaddr_in server = {0};
	
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	
	if (inet_pton(AF_INET, ip, &server.sin_addr) <= 0) {
		printf("cb_nb_init: inet_pton failed\n");
		close(nb->fd);
		return false;
	}
	
	if (connect(nb->fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
		printf("cb_nb_init: connect failed\n");
		close(nb->fd);
		return false;
	}
	
	return true;
}

void cb_nb_free(struct cb_nb* nb) {
	close(nb->fd);
}

bool cb_nb_read(struct cb_nb* nb, void* dst, size_t size) {
	size_t i = 0;
	while (i < size) {
		int res = read(nb->fd, (uint8_t*)dst + i, size - i);
		if (res < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
			printf("cb_nb_read: read failed\n");
			return false;
		}
		i += res;
	}
	return true;
}
bool cb_nb_write(struct cb_nb* nb, void* src, size_t size) {
	size_t i = 0;
	while (i < size) {
		int res = write(nb->fd, (uint8_t*)src + i, size - i);
		if (res < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
			printf("cb_nb_write: write failed\n");
			return false;
		}
		i += res;
	}
	return true;
}

bool cb_nb_rbyte(struct cb_nb* nb, uint8_t* v) {
	if (!cb_nb_read(nb, v, 1)) {
		printf("cb_nb_rbyte: cb_nb_read failed\n");
		return false;
	}
	return true;
}
bool cb_nb_wbyte(struct cb_nb* nb, uint8_t v) {
	if (!cb_nb_write(nb, &v, 1)) {
		printf("cb_nb_wbyte: cb_nb_write failed\n");
		return false;
	}
	return true;
}

bool cb_nb_rshort(struct cb_nb* nb, uint16_t* v) {
	if (!cb_nb_read(nb, v, 2)) {
		printf("cb_nb_rshort: cb_nb_read failed\n");
		return false;
	}
	*v = ntohs(*v);
	return true;
}
bool cb_nb_wshort(struct cb_nb* nb, uint16_t v) {
	v = htons(v);
	if (!cb_nb_write(nb, &v, 2)) {
		printf("cb_nb_wshort: cb_nb_write failed\n");
		return false;
	}
	return true;
}

bool cb_nb_rint(struct cb_nb* nb, uint32_t* v) {
	if (!cb_nb_read(nb, v, 4)) {
		printf("cb_nb_rint: cb_nb_read failed\n");
		return false;
	}
	*v = ntohl(*v);
	return true;
}
bool cb_nb_wint(struct cb_nb* nb, uint32_t v) {
	v = htonl(v);
	if (!cb_nb_write(nb, &v, 4)) {
		printf("cb_nb_wint: cb_nb_write failed\n");
		return false;
	}
	return true;
}

bool cb_nb_rlong(struct cb_nb* nb, uint64_t* v) {
	if (!cb_nb_read(nb, &v, 8)) {
		printf("cb_nb_rlong: cb_nb_read failed\n");
		return false;
	}
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	*v = __builtin_bswap64(*v);
	#endif
	return true;
}
bool cb_nb_wlong(struct cb_nb* nb, uint64_t v) {
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	v = __builtin_bswap64(v);
	#endif
	if (!cb_nb_write(nb, &v, 8)) {
		printf("cb_nb_wlong: cb_nb_write failed\n");
		return false;
	}
	return true;
}

bool cb_nb_rstring16(struct cb_nb* nb, char** v) {
	uint16_t len;
	if (!cb_nb_rshort(nb, &len)) {
		printf("cb_nb_rstring16: cb_nb_rshort failed\n");
		return false;
	}
	char* buf = malloc(len + 1);
	if (!buf) {
		printf("cb_nb_rstring16: malloc failed\n");
		return false;
	}
	for (uint16_t i=0; i<len; i++) {
		uint16_t c;
		if (!cb_nb_rshort(nb, &c)) {
			printf("cb_nb_rstring16: cb_nb_rshort failed\n");
			free(buf);
			return false;
		}
		buf[i] = (char)c;
	}
	*v = buf;
	return true;
}
bool cb_nb_wstring16(struct cb_nb* nb, const char* v) {
	size_t len = strlen(v);
	if (len >= 65536) {
		printf("cb_nb_wstring16: string too large (%llu bytes)\n", (unsigned long long)len);
		return false;
	}
	if (!cb_nb_wshort(nb, (uint16_t)len)) {
		printf("cb_nb_wstring16: cb_nb_wshort failed\n");
		return false;
	}
	for (size_t i=0; i<len; i++) {
		if (!cb_nb_wshort(nb, (uint16_t)v[i])) {
			printf("cb_nb_wstring16: cb_nb_wshort failed\n");
			return false;
		}
	}
	return true;
}
