#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <string>
#include "ObjectType.h"
#include "Animator.h"

class Sprite
{
public:
	Sprite();
	Sprite(std::string spriteName, int x, int y, const char* imgfile, bool show, std::pair<double, double> velo);
	Sprite(std::string spriteName, int x, int y, const char* imgfile, bool show, std::pair<double, double> velo, objectType stype);
	void Update();
	void setPos(int x, int y);
	void chanPos(int x, int y);
	void move();
	void setAnimator(int fullW, int fullH, std::vector<std::pair<int, int>> sheetSizes, std::vector<int> sheetCounts);
	std::pair<std::pair<int, int>, std::pair<int, int>> curAni();
	std::pair<std::pair<int, int>, std::pair<int, int>> getSheet(int order, int count);
	void loopAniWhen(int order, bool);
	std::string name;
	objectType type;
	int x;
	int y;
	double a;
	double s;
	std::pair<double, double> v;
	bool visible;
	const char* img;
	virtual ~Sprite();
private:
	Animator animator;
};

#endif