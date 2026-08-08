// v3.h

#ifndef CBETA_V3_H
#define CBETA_V3_H

#include <math.h>
#include <stdbool.h>

struct cb_loc;

struct cb_v3 {
	float x;
	float y;
	float z;
};

static inline struct cb_v3 cb_v3(float x, float y, float z) {
	return (struct cb_v3){x, y, z};
}

struct cb_v3 cb_v3_addv(struct cb_v3 v1, struct cb_v3 v2);
struct cb_v3 cb_v3_subv(struct cb_v3 v1, struct cb_v3 v2);
struct cb_v3 cb_v3_mulv(struct cb_v3 v1, struct cb_v3 v2);
struct cb_v3 cb_v3_divv(struct cb_v3 v1, struct cb_v3 v2);

struct cb_v3 cb_v3_adds(struct cb_v3 v, float s);
struct cb_v3 cb_v3_subs(struct cb_v3 v, float s);
struct cb_v3 cb_v3_muls(struct cb_v3 v, float s);
struct cb_v3 cb_v3_divs(struct cb_v3 v, float s);

float cb_v3_length2(struct cb_v3 v);
float cb_v3_length(struct cb_v3 v);

struct cb_v3 cb_v3_normalize(struct cb_v3 v);

#endif
