#ifndef HITBOX_H
#define HITBOX_H

#include "Sprite.h"
#include "ObjectType.h"

class Hitbox
{
public:
	Hitbox();
	Hitbox(Hitbox&, Sprite*);
	Hitbox(int width, int height, Sprite* hitSprite, double changeX, double changeY);
	void setHitbox(int width, int height, Sprite* hitSprite, double changeX, double changeY);
	void LinkTo(Sprite* hitSprite, double changeX, double changeY);
	bool IsOn(Hitbox* hit2);
	bool IsOnExtended(Hitbox* hit2, int ext);
	bool pointInBox(int xpos, int ypos);
	void Update();
	int w;
	int h;
	double s;
	int x;
	int y;
	double chanX;
	double chanY;
	bool active;
	objectType type;
	Sprite* sprite;
};

#endif
