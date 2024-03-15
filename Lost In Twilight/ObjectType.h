#ifndef OBJECT_TYPE_H
#define OBJECT_TYPE_H

enum objectType {
	DEFAULT,
	C_PLAYER,
	BULLET,
	B_PLAYER,
	WALL,
	W_EDGE
};

bool operator==(objectType a, objectType b);

#endif