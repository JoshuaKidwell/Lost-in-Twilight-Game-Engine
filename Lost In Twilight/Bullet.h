#ifndef BULLET_H
#define BULLET_H
#include "Sprite.h"
#include "ObjectType.h"

class Bullet : public Sprite
{
public:
	Bullet(std::string spriteName, double x, double y, const char* imgfile, bool show, std::pair<double, double> velo, objectType btype);
	Bullet(std::string spriteName, Bullet&, double x, double y, std::pair<double, double> velo);
private:
	int bounces;
	int hits;
};

#endif