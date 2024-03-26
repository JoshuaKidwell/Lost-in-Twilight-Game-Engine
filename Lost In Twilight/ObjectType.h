#ifndef OBJECT_TYPE_H
#define OBJECT_TYPE_H

enum objectType {
	DEFAULT,
	C_PLAYER,
	C_ENEMY,
	BULLET,
	B_PLAYER,
	B_ENEMY,
	WALL,
	W_EDGE
};

bool operator==(objectType a, objectType b);
bool operator!=(objectType a, objectType b);

#endif