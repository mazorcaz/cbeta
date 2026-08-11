// main.c

#include <cbeta/main.h>

#include <stdio.h>
#include <stdlib.h>
#include <cbeta/engine.h>
#include <cbeta/packet.h>
#include <cbeta/nb.h>
#include <cbeta/util.h>

int main() {
	struct cb_nb nb;
	if (!cb_nb_init(&nb, "147.185.221.224", 10793)) {
		printf("main: cb_nb_init failed\n");
		return -1;
	}
	
	{
		struct cb_packet_handshake* packet = cb_packet_create(CB_PACKET_HANDSHAKE);
		packet->string0 = cb_strdup("username");
		cb_packet_write(packet, &nb);
		cb_packet_free(packet);
	}
	{
		struct cb_packet* packet = cb_packet_read(&nb);
		printf("%02x\n", packet->id);
		cb_packet_free(packet);
	}
	{
		struct cb_packet_login_request* packet = cb_packet_create(CB_PACKET_LOGIN_REQUEST);
		packet->int0 = 14;
		packet->string0 = cb_strdup("username");
		packet->long0 = 0;
		packet->byte0 = 0;
		cb_packet_write(packet, &nb);
		cb_packet_free(packet);
	}
	{
		struct cb_packet* packet = cb_packet_read(&nb);
		printf("%02x\n", packet->id);
		cb_packet_free(packet);
	}
	
	cb_nb_free(&nb);
	
	/*
	struct cb_engine* engine = malloc(sizeof(struct cb_engine));
	if (!engine || !cb_engine_init(engine)) {
		printf("main: it didnt work :`(\n");
		return -1;
	}
	
	cb_engine_run(engine);
	
	cb_engine_free(engine);
	free(engine); */

	return 0;
}
