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
	isInverse = false;
	Update();
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
	isInverse = hit.isInverse;
	Update();
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
	isInverse = false;
	Update();
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
	isInverse = false;
	Update();
}

void Hitbox::LinkTo(Sprite* hitSprite, double changeX, double changeY)
{
	chanX = changeX;
	chanY = changeY;
	sprite = hitSprite;
	Update();
}

bool Hitbox::IsOn(Hitbox* hit2)
{
	//compare all points to eachother of each hitbox
	if (pointInBox(hit2->p[0].first, hit2->p[0].second) || pointInBox(hit2->p[1].first, hit2->p[1].second) || pointInBox(hit2->p[2].first, hit2->p[2].second) || pointInBox(hit2->p[3].first, hit2->p[3].second)) {
		return true;
	}

	if (hit2->pointInBox(p[0].first, p[0].second) || hit2->pointInBox(p[1].first, p[1].second) || hit2->pointInBox(p[2].first, p[2].second) || hit2->pointInBox(p[3].first, p[3].second)) {
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
	hit.setPoints();

	return (this->IsOn(&hit));
}

bool Hitbox::pointInBox(int xpos, int ypos)
{
	//point out of box
	if (isInverse) {
		if (!((x + w >= xpos && x <= xpos) && (y + h >= ypos && y <= ypos))) {
			return true;
		}
		return false;
	}

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

	setPoints();
}

void Hitbox::setPoints()
{
	p[0].first = x;
	p[0].second = y;
	p[1].first = x + w;
	p[1].second = y;
	p[2].first = x + w;
	p[2].second = y + h;
	p[3].first = x;
	p[3].second = y + h;
}
