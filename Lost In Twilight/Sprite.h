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
	Sprite(std::string spriteName, double x, double y, const char* imgfile, bool show);
	Sprite(std::string spriteName, double x, double y, const char* imgfile, bool show, objectType stype);
	void Update();
	void setPos(double x, double y);
	void chanPos(double x, double y);
	void move();
	void setAnimator(int fullW, int fullH, std::vector<std::pair<int, int>> sheetSizes, std::vector<int> sheetCounts);
	std::pair<std::pair<int, int>, std::pair<int, int>> curAni();
	std::pair<std::pair<int, int>, std::pair<int, int>> getSheet(int order, int count);
	void loopAniWhen(int order, bool);

	std::string name;
	objectType type;
	double x;
	double y;
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