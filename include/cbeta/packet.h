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
#define CB_PACKET_HOLDING_CHANGE 0x10 // 16
#define CB_PACKET_USE_BED 0x11 // 17
#define CB_PACKET_ANIMATION 0x12 // 18
#define CB_PACKET_ENTITY_ACTION 0x13 // 19
#define CB_PACKET_ENTITY_SPAWN 0x14 // 20
#define CB_PACKET_ITEM_SPAWN 0x15 // 21
#define CB_PACKET_COLLECT_ITEM 0x16 // 22
#define CB_PACKET_VEHICLE_SPAWN 0x17 // 23
#define CB_PACKET_MOB_SPAWN 0x18 // 24
#define CB_PACKET_PAINTING 0x19 // 25
#define CB_PACKET_STANCE_UPDATE 0x1B // 27
#define CB_PACKET_ENTITY_VELOCITY 0x1C // 28
#define CB_PACKET_DESTROY_ENTITY 0x1D // 29
#define CB_PACKET_ENTITY 0x1E // 30
#define CB_PACKET_ENTITY_MOVE 0x1F // 31
#define CB_PACKET_ENTITY_LOOK 0x20 // 32
#define CB_PACKET_ENTITY_LOOK_MOVE 0x21 // 33
#define CB_PACKET_ENTITY_TELEPORT 0x22 // 34
#define CB_PACKET_ENTITY_STATUS 0x26 // 38
#define CB_PACKET_ENTITY_RIDE 0x27 // 39
#define CB_PACKET_ENTITY_METADATA 0x28 // 40
#define CB_PACKET_ENTITY_PRE_CHUNK 0x32 // 50
#define CB_PACKET_ENTITY_MAP_CHUNK 0x33 // 51
#define CB_PACKET_MULTI_BLOCK_CHANGE 0x34 // 52
#define CB_PACKET_BLOCK_CHANGE 0x35 // 53
#define CB_PACKET_BLOCK_ACTION 0x36 // 54
#define CB_PACKET_EXPLOSION 0x3C // 60
#define CB_PACKET_SOUND_EFFECT 0x3D // 61
#define CB_PACKET_STATE_CHANGE 0x46 // 70
#define CB_PACKET_THUNDERBOLT 0x47 // 71
#define CB_PACKET_OPEN_WINDOW 0x64 // 100
#define CB_PACKET_CLOSE_WINDOW 0x65 // 101
#define CB_PACKET_WINDOW_CLICK 0x66 // 102
#define CB_PACKET_SET_SLOT 0x67 // 103
#define CB_PACKET_WINDOW_ITEMS 0x68 // 104
#define CB_PACKET_PROGRESS_BAR 0x69 // 105
#define CB_PACKET_TRANSACTION 0x6A // 106
#define CB_PACKET_UPDATE_SIGN 0x82 // 130
#define CB_PACKET_ITEM_DATA 0x83 // 131
#define CB_PACKET_INCREMENT_STATISTIC 0xC8 // 200
#define CB_PACKET_DISCONNECT 0xFF // 255

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
