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
	Sprite(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show);
	Sprite(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show, objectType stype);
	Sprite(std::string spriteName, double xpos, double ypos, const char* imgfile, bool show, objectType stype, double scale);
	Sprite(std::string spriteName, Sprite&);
	void Update(double Delta);
	void setPos(double x, double y);
	void chanPos(double x, double y);
	void move(double delta);
	void setAnimator(int fullW, int fullH, std::vector<std::pair<int, int>> sheetSizes, std::vector<int> sheetCounts);
	std::pair<std::pair<int, int>, std::pair<int, int>> curAni();
	std::pair<std::pair<int, int>, std::pair<int, int>> getSheet(int order, int count);
	void setAnimationOrder(int order);
	void nextAnimationWhen(int order, bool);
	void nextAnimation(int order);
	void nextAnimation();
	void nextAnimationWhen(bool);
	int getFrameNum();

	std::string name;
	objectType type;
	double x;
	double y;
	double a;
	double angleOffset;
	double s;
	std::pair<double, double> v;
	bool visible;
	const char* img;

	virtual ~Sprite();
protected:
	int animationOrder;
	Animator animator;
};

#endif