#include "ObjectType.h"

bool operator==(objectType a, objectType b) 
{
	//make bullets equal eachother
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

	//make walls equal eachother
	if (static_cast<int>(a) == static_cast<int>(WALL)) {
		switch (static_cast<int>(b)) {
		case static_cast<int>(W_EDGE):
			return true;
		}
	}
	else if (static_cast<int>(b) == static_cast<int>(WALL)) {
		switch (static_cast<int>(a)) {
		case static_cast<int>(W_EDGE):
			return true;
		}
	}

	return static_cast<int>(a) == static_cast<int>(b);
}
