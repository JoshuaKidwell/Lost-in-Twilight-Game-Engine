#include "Sprite.h"
#include <math.h>
#include <SDL_image.h>
#include <iostream>

Sprite::Sprite()
{
	name = "Jeff";
	x = 0;
	y = 0;
	visible = true;
	v.first = 0;
	v.second = 0;
	a = 0;
}

Sprite::Sprite(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show)
{
	name = spriteName;
	x = xpos;
	y = ypos;
	img = imgfile;
	visible = show;
	v.first = 0;
	v.second = 0;
	a = 0;
	s = 1;
	type = DEFAULT;
}

Sprite::Sprite(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show, objectType stype)
{
	name = spriteName;
	x = xpos;
	y = ypos;
	img = imgfile;
	visible = show;
	v.first = 0;
	v.second = 0;
	a = 0;
	s = 1;
	type = stype;
}

void Sprite::Update()
{
	move();
}

void Sprite::setPos(double xpos, double ypos) { x = xpos; y = ypos; }

void Sprite::chanPos(double xpos, double ypos) { setPos(x + xpos, y + ypos); }

void Sprite::move()
{
	x += v.first;
	y += v.second;
}

void Sprite::setAnimator(int fullW, int fullH, std::vector<std::pair<int, int>> sheetSizes, std::vector<int> sheetCounts)
{
	animator.setAnimator(fullW, fullH, sheetSizes, sheetCounts);
}

std::pair<std::pair<int, int>, std::pair<int, int>> Sprite::curAni()
{
	return animator.current;
}

std::pair<std::pair<int, int>, std::pair<int, int>> Sprite::getSheet(int order, int count)
{
	return animator.getSheet(order, count);
}

void Sprite::loopAniWhen(int order, bool t)
{
	if (t)
		animator.loop(order);
}

Sprite::~Sprite()
{
}
