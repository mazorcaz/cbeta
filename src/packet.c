// packet.c

#include <cbeta/packet.h>

#include <stdlib.h>
#include <stdio.h>
#include <cbeta/nb.h>

void* cb_packet_init(uint8_t id)
{
	void* p;
	switch (id) {
		case CB_PACKET_LOGIN_REQUEST: {
			p = malloc(sizeof(struct cb_packet_login_request));
			break;
		}
		case CB_PACKET_HANDSHAKE: {
			p = malloc(sizeof(struct cb_packet_handshake));
			break;
		}
		default: {
			printf("cb_packet_init: invalid packet %02x\n", id);
			return NULL;
		}
	}
	if (p == NULL) {
		printf("cb_packet_init: malloc failed\n");
		return NULL;
	}
	struct cb_packet* generic = p;
	generic->id = id;
	return p;
}

void cb_packet_free(void* packet)
{
	struct cb_packet* generic = packet;
	switch (generic->id) {
		case CB_PACKET_LOGIN_REQUEST: {
			struct cb_packet_login_request* p = packet;
			free(p->string0);
			break;
		}
		case CB_PACKET_HANDSHAKE: {
			struct cb_packet_handshake* p = packet;
			free(p->string0);
			break;
		}
	}
	free(packet);		
}

void* cb_packet_read(struct cb_nb* nb)
{
	uint8_t id;
	if (!cb_nb_rbyte(nb, &id)) goto fail;
	void* generic = cb_packet_init(id);
	if (!generic) {
		printf("cb_packet_read: cb_packet_init failed\n");
		return NULL;
	}
	switch (id) {
		case CB_PACKET_LOGIN_REQUEST: {
			struct cb_packet_login_request* p = generic;
			if (!cb_nb_rint(nb, &p->int0)) goto fail;
			if (!cb_nb_rstring16(nb, &p->string0)) goto fail;
			if (!cb_nb_rlong(nb, &p->long0)) goto fail;
			if (!cb_nb_rbyte(nb, &p->byte0)) goto fail;
			break;
		}
		case CB_PACKET_HANDSHAKE: {
			struct cb_packet_handshake* p = generic;
			if (!cb_nb_rstring16(nb, &p->string0)) goto fail;
			break;
		}
		default: {
			printf("cb_packet_read: invalid packet %02x\n", id);
			return NULL;
		}
	}
	return generic;
	
	fail:
	printf("cb_packet_read: nb_cb_read failed\n");
	return NULL;
}

bool cb_packet_write(void* packet, struct cb_nb* nb)
{
	struct cb_packet* generic = packet;
	if (!cb_nb_wbyte(nb, generic->id)) goto fail;
	switch (generic->id) {
		case CB_PACKET_LOGIN_REQUEST: {
			struct cb_packet_login_request* p = packet;
			if (!cb_nb_wint(nb, p->int0)) goto fail;
			if (!cb_nb_wstring16(nb, p->string0)) goto fail;
			if (!cb_nb_wlong(nb, p->long0)) goto fail;
			if (!cb_nb_wbyte(nb, p->byte0)) goto fail;
			break;
		}
		case CB_PACKET_HANDSHAKE: {
			struct cb_packet_handshake* p = packet;
			if (!cb_nb_wstring16(nb, p->string0)) goto fail;
			break;
		}
		default: {
			printf("cb_packet_write: invalid packet %02x\n", generic->id);
			return false;
		}
	}
	return true;
	
	fail:
	printf("cb_packet_write: cb_nb_write failed\n");
	return false;
}
