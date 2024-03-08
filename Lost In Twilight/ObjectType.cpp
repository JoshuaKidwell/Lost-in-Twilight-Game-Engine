#include "ObjectType.h"

bool operator==(objectType a, objectType b) 
{
	if (static_cast<int>(a) == static_cast<int>(BULLET)) {
		switch (static_cast<int>(b)) {
		case static_cast<int>(B_PLAYER):
			return true;
		}
	}
	else if (static_cast<int>(b) == static_cast<int>(BULLET)) {
		switch (static_cast<int>(a)) {
		case static_cast<int>(B_PLAYER):
			return true;
		}
	}

	return static_cast<int>(a) == static_cast<int>(b);
}
