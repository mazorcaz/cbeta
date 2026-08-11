// packet.h

#ifndef CBETA_PACKET_H
#define CBETA_PACKET_H

#include <stdbool.h>
#include <stdint.h>

struct cb_nb;

#define CB_PACKET_KEEP_ALIVE 0x00 // 0
#define CB_PACKET_LOGIN_REQUEST 0x01 // 1
#define CB_PACKET_HANDSHAKE 0x02 // 2
#define CB_PACKET_CHAT_MESSAGE 0x03 // 3
#define CB_PACKET_TIME_UPDATE 0x04 // 4
#define CB_PACKET_ENTITY_EQUIPMENT 0x05 // 5
#define CB_PACKET_SPAWN_POSITION 0x06 // 6
#define CB_PACKET_USE_ENTITY 0x07 // 7
#define CB_PACKET_UPDATE_HEALTH 0x08 // 8
#define CB_PACKET_RESPAWN 0x09 // 9 
#define CB_PACKET_PLAYER 0x0A // 10
#define CB_PACKET_PLAYER_POSTION 0x0B // 11
#define CB_PACKET_PLAYER_LOOK 0x0C // 12
#define CB_PACKET_PLAYER_POSITION_LOOK 0x0D // 13
#define CB_PACKET_PLAYER_DIG 0x0E // 14
#define CB_PACKET_PLAYER_PLACE_BLOCK 0x0F // 15

// packet owns all buffers
// cb_packet_free() must be called when done with a packet.
// CB_PACKET_FREE CALLS FREE() DO NOT CALL FREE YOURSELF

struct cb_packet {
	uint8_t id;
};

struct cb_packet_login_request {
	uint8_t id;
    uint32_t int0;
    char* string0;
    uint64_t long0;
    uint8_t byte0;
};

struct cb_packet_handshake {
	uint8_t id;
    char* string0;
};

struct cb_packet_chat_message {
	uint8_t id;
    char* string0;
};

struct cb_packet_time_update {
	uint8_t id;
    uint64_t long0;
};

struct cb_packet_entity_equipment {
	uint8_t id;
    uint32_t int0;
    uint16_t short0;
    uint16_t short1;
    uint16_t short2;
};

struct cb_packet_spawn_position {
	uint8_t id;
    uint32_t int0;
    uint32_t int1;
    uint32_t int2;
};

void* cb_packet_init(uint8_t id);
void cb_packet_free(void* packet);

// calls cb_packet_init before
void* cb_packet_read(struct cb_nb* nb);

bool cb_packet_write(void* packet, struct cb_nb* nb);

#endif
