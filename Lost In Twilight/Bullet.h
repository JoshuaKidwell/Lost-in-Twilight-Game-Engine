#ifndef BULLET_H
#define BULLET_H
#include "Sprite.h"
#include "ObjectType.h"

class Bullet : public Sprite
{
public:
	Bullet(std::string spriteName, int x, int y, const char* imgfile, bool show, std::pair<double, double> velo, objectType btype);
private:
	int bounces;
	int hits;
};

#endif