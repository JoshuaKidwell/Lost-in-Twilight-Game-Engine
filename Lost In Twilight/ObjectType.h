#ifndef OBJECT_TYPE_H
#define OBJECT_TYPE_H

enum objectType {
	DEFAULT,
	F_PLAYER,
	BULLET,
	B_PLAYER
};

bool operator==(objectType a, objectType b);

#endif