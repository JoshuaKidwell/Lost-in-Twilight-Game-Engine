#ifndef HITBOX_H
#define HITBOX_H

#include "Sprite.h"
#include "ObjectType.h"

class Hitbox
{
public:
	Hitbox();
	Hitbox(int width, int height, Sprite* hitSprite, int changeX, int changeY);
	void setHitbox(int width, int height, Sprite* hitSprite, int changeX, int changeY);
	void LinkTo(Sprite* hitSprite, int changeX, int changeY);
	bool IsOn(Hitbox* hit2);
	bool pointInBox(int xpos, int ypos);
	void Update();
	int w;
	int h;
	int x;
	int y;
	objectType type;
private:
	Sprite* sprite;
	int chanX;
	int chanY;
};

#endif
