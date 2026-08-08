// v3.c

#include <cbeta/v3.h>

#include <math.h>

struct cb_v3 cb_v3_addv(struct cb_v3 v1, struct cb_v3 v2) {
	return cb_v3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}
struct cb_v3 cb_v3_subv(struct cb_v3 v1, struct cb_v3 v2) {
	return cb_v3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}
struct cb_v3 cb_v3_mulv(struct cb_v3 v1, struct cb_v3 v2) {
	return cb_v3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}
struct cb_v3 cb_v3_divv(struct cb_v3 v1, struct cb_v3 v2) {
	return cb_v3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}

struct cb_v3 cb_v3_adds(struct cb_v3 v, float s) {
	return cb_v3(v.x+s, v.y+s, v.z+s);
}
struct cb_v3 cb_v3_subs(struct cb_v3 v, float s) {
	return cb_v3(v.x-s, v.y-s, v.z-s);
}
struct cb_v3 cb_v3_muls(struct cb_v3 v, float s) {
	return cb_v3(v.x*s, v.y*s, v.z*s);
}
struct cb_v3 cb_v3_divs(struct cb_v3 v, float s) {
	return cb_v3(v.x/s, v.y/s, v.z/s);
}

float cb_v3_length2(struct cb_v3 v) {
	return v.x*v.x + v.y*v.y + v.z*v.z;
}
float cb_v3_length(struct cb_v3 v) {
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

struct cb_v3 cb_v3_normalize(struct cb_v3 v) {
	return cb_v3_divs(v, cb_v3_length(v));
}
