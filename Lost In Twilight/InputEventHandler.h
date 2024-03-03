#ifndef INPUTEVENTHANDLER_H
#define INPUTEVENTHANDLER_H

#include <SDL.h>
#include "GlobalVariables.h"
#include <map>
#include <vector>

class InputEventHandler
{
public:
	InputEventHandler();
	void setFps(int* fps);
	void updateInputs(int, int);

	//Accessable Key information true=presssed/false=unpressed
	bool exit;
	bool f11;
	bool f11c;
	bool w;
	bool s;
	bool d;
	bool a;

	bool ml;
	bool mlc;
	bool mr;
	bool mrc;
	int mx;
	int my;

	int frameCount;
	double secCount;
	bool wait(double sec, int specialKey);
	int* FPS;
private:
	void updateClicks(bool, bool&, int);
	std::vector<bool> u;

	std::map<std::pair<double, int>, int> waitMap;
	SDL_Event event;
};

#endif