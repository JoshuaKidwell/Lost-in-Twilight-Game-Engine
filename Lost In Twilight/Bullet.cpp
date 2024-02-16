#include "Bullet.h"
#include <iostream>

Bullet::Bullet(std::string spriteName, int xpos, int ypos, const char* imgfile, bool show, std::pair<double, double> velo, objectType btype) 
{ 
	name = spriteName;
	x = xpos;
	y = ypos;
	img = imgfile;
	visible = show;
	v.first = velo.first;
	v.second = velo.second;
	a = 0;
	bounces = 0;
	hits = 0;
	type = btype;
}
