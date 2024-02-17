#include "Bullet.h"
#include <iostream>

Bullet::Bullet(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show, std::pair<double, double> velo, objectType btype) 
{ 
	name = spriteName;
	x = xpos;
	y = ypos;
	img = imgfile;
	visible = show;
	v.first = velo.first;
	v.second = velo.second;
	a = 0;
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
	v.first = velo.first;
	v.second = velo.second;
	a = bullet.a;
	s = bullet.s;
	animator = bullet.animator;

	bounces = bullet.bounces;
	hits = bullet.hits;
	type = bullet.type;
}
