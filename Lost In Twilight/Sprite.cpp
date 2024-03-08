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
	angleOffset = 0;
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
	angleOffset = 0;
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
	angleOffset = 0;
	s = 1;
	type = stype;
}

Sprite::Sprite(std::string spriteName, Sprite& sprite)
{
	name = spriteName;
	x = sprite.x;
	y = sprite.y;
	img = sprite.img;
	visible = sprite.s;
	v.first = 0;
	v.second = 0;
	a = sprite.a;
	angleOffset = sprite.angleOffset;
	s = sprite.s;
	type = sprite.type;
	animator = sprite.animator;
}

void Sprite::Update(double delta)
{
	move(delta);

	//set angle between 180 and -180
	if (a > 180 || a < -180) {
		a = a - (int)a + (((int)a + 180) % 360 - 180);
	}
}

void Sprite::setPos(double xpos, double ypos) { x = xpos; y = ypos; }

void Sprite::chanPos(double xpos, double ypos) { setPos(x + xpos, y + ypos); }

void Sprite::move(double delta)
{
	x += v.first * delta;
	y += v.second * delta;
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

void Sprite::setAnimationOrder(int order)
{
	animationOrder = order;
}

void Sprite::nextAnimationWhen(int order, bool t)
{
	animationOrder = order;
	if (t)
		animator.loop(order);
}

void Sprite::nextAnimation(int order)
{
	animationOrder = order;
	animator.loop(order);
}

void Sprite::nextAnimation()
{
	animator.loop(animationOrder);
}

void Sprite::nextAnimationWhen(bool t)
{
	if (t)
		animator.loop(animationOrder);
}

int Sprite::getFrameNum()
{
	return animator.looping_count;
}

Sprite::~Sprite()
{
}
