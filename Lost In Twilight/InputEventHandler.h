#ifndef INPUTEVENTHANDLER_H
#define INPUTEVENTHANDLER_H

#include <SDL.h>
#include <map>

class InputEventHandler
{
public:
	InputEventHandler();
	void updateInputs();

	//Accessable Key information true=presssed/false=unpressed
	bool exit;
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
	int FPS;
private:
	void updateClicks(bool, bool&, bool&);
	bool umlc;
	bool umrc;
	std::map<std::pair<double, int>, int> waitMap;
	SDL_Event event;
};

#endif