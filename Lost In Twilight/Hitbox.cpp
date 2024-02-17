#include "Hitbox.h"
#include "Sprite.h"

Hitbox::Hitbox()
{
	w = 20;
	h = 20;
	sprite = nullptr;
	chanX = 0;
	chanY = 0;
}

Hitbox::Hitbox(Hitbox& hit, Sprite* hitSprite)
{
	w = hit.w;
	h = hit.h;
	chanX = hit.chanX;
	chanY = hit.chanY;
	sprite = hitSprite;
	type = hit.type;
}

Hitbox::Hitbox(int width, int height, Sprite* hitSprite, int changeX, int changeY)
{
	w = width;
	h = height;
	chanX = changeX;
	chanY = changeY;
	sprite = hitSprite;
	type = sprite->type;
}

void Hitbox::setHitbox(int width, int height, Sprite* hitSprite, int changeX, int changeY)
{
	w = width;
	h = height;
	chanX = changeX;
	chanY = changeY;
	sprite = hitSprite;
}

void Hitbox::LinkTo(Sprite* hitSprite, int changeX, int changeY)
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
	return false;
}

bool Hitbox::pointInBox(int xpos, int ypos)
{
	if ((x + w >= xpos && x <= xpos) && (y + h >= ypos && y <= ypos)) {
		return true;
	}
	return false;
}

void Hitbox::Update()
{
	x = sprite->x + chanX;
	y = sprite->y + chanY;
}
