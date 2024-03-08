#include "Hitbox.h"
#include "Sprite.h"

#include <math.h>
#include <iostream>

Hitbox::Hitbox()
{
	s = 1;
	w = 20;
	h = 20;
	sprite = nullptr;
	chanX = 0;
	chanY = 0;
	active = true;
}

Hitbox::Hitbox(Hitbox& hit, Sprite* hitSprite)
{
	s = hit.s;
	w = hit.w;
	h = hit.h;
	chanX = hit.chanX;
	chanY = hit.chanY;
	sprite = hitSprite;
	type = hit.type;
	x = hit.x;
	y = hit.y;
	active = true;
}

Hitbox::Hitbox(int width, int height, Sprite* hitSprite, double changeX, double changeY)
{
	s = hitSprite->s;
	w = width * s;
	h = height * s;
	chanX = changeX;
	chanY = changeY;
	sprite = hitSprite;
	type = sprite->type;
	active = true;
}

void Hitbox::setHitbox(int width, int height, Sprite* hitSprite, double changeX, double changeY)
{
	s = hitSprite->s;
	w = width * s;
	h = height * s;
	chanX = changeX;
	chanY = changeY;
	sprite = hitSprite;
	active = true;
}

void Hitbox::LinkTo(Sprite* hitSprite, double changeX, double changeY)
{
	chanX = changeX;
	chanY = changeY;
	sprite = hitSprite;
}

bool Hitbox::IsOn(Hitbox* hit2)
{
	int xpos1 = hit2->x;
	int ypos1 = hit2->y;
	int xpos2 = hit2->x + hit2->w;
	int ypos2 = hit2->y;
	int xpos3 = hit2->x + hit2->w;
	int ypos3 = hit2->y + hit2->h;
	int xpos4 = hit2->x;
	int ypos4 = hit2->y + hit2->h;
	if (pointInBox(xpos1, ypos1) || pointInBox(xpos2, ypos2) || pointInBox(xpos3, ypos3) || pointInBox(xpos4, ypos4)) {
		return true;
	}

	xpos1 = x;
	ypos1 = y;
	xpos2 = x + w;
	ypos2 = y;
	xpos3 = x + w;
	ypos3 = y + h;
	xpos4 = x;
	ypos4 = y + h;
	if (hit2->pointInBox(xpos1, ypos1) || hit2->pointInBox(xpos2, ypos2) || hit2->pointInBox(xpos3, ypos3) || hit2->pointInBox(xpos4, ypos4)) {
		return true;
	}

	return false;
}

bool Hitbox::IsOnExtended(Hitbox* hit2, int ext)
{
	Hitbox hit(*hit2, hit2->sprite);
	hit.w += ext * 2;
	hit.h += ext * 2;
	hit.x -= ext;
	hit.y -= ext;

	return (this->IsOn(&hit));
}

bool Hitbox::pointInBox(int xpos, int ypos)
{
	if ((x + w >= xpos && x <= xpos) && (y + h >= ypos && y <= ypos)) {
		return true;
	}
	return false;
}

double Hitbox::AngleOut(Hitbox* CH)
{
	//Could have error with angles not -180 to 180
	//CH is the moving hit box
	if (CH->y > y && CH->y + CH->h < y + h) {
		return -CH->sprite->a + 180;
	}
	else if (CH->x > x && CH->x + CH->w < x + w) {
		return -CH->sprite->a;
	}
	else if ((CH->x < x && CH->y < y) || (CH->x < x && CH->y < y + h)) {
		if (CH->sprite->a <= 45 && CH->sprite->a >= -45) {
			return -CH->sprite->a + 180;
		}
		else {
			return -CH->sprite->a;
		}
	}
	else if ((CH->x > x && CH->y < y) || (CH->x > x && CH->y < y + h)) {
		if (CH->sprite->a >= 135 && CH->sprite->a <= -135) {
			return -CH->sprite->a + 180;
		}
		else {
			return -CH->sprite->a;
		}
	}

	return 90;
}

void Hitbox::Update()
{
	int xr = chanX * s;
	int yr = chanY * s;

	//finds point of the center of the rotated section in the sprite, then adjusts acordingly

	x = sprite->x - w/2 + (xr * cos((sprite->a + sprite->angleOffset) * 3.1415 / 180) - yr * sin((sprite->a + sprite->angleOffset) * 3.1415 / 180));
	y = sprite->y - h/2 + (yr * cos((sprite->a + sprite->angleOffset) * 3.1415 / 180) + xr * sin((sprite->a + sprite->angleOffset) * 3.1415 / 180));
}
