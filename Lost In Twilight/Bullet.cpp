#include "Bullet.h"
#include <iostream>

Bullet::Bullet(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show, objectType stype) : Sprite(spriteName, xpos, ypos, imgfile, show, stype)
{
	bounces = 0;
	hits = 0;
}

Bullet::Bullet(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show, objectType stype, double scale) : Sprite(spriteName, xpos, ypos, imgfile, show, stype, scale)
{
	bounces = 0;
	hits = 0;
}

Bullet::Bullet(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show, std::pair<double, double> velo, objectType btype)
{ 
	name = spriteName;
	x = xpos;
	y = ypos;
	img = imgfile;
	visible = show;
	v = velo;
	a = 0;
	angleOffset = 0;
	s = 0;

	bounces = 0;
	hits = 0;
	type = btype;
}

Bullet::Bullet(std::string spriteName, Bullet& bullet, double xpos, double ypos, std::pair<double, double> velo)
{
	name = spriteName;
	x = xpos;
	y = ypos;
	img = bullet.img;
	visible = bullet.s;
	v = velo;
	a = bullet.a;
	s = bullet.s;
	angleOffset = bullet.angleOffset;
	animator = bullet.animator;

	bounces = bullet.bounces;
	hits = bullet.hits;
	type = bullet.type;
}
