// loc.h

#ifndef CBETA_LOC_H
#define CBETA_LOC_H

struct cb_loc {
	int x;
	int y;
	int z;
};

static inline struct cb_loc cb_loc(int x, int y, int z) {
	return (struct cb_loc){x, y, z};
}

#endif
