#include "InputEventHandler.h"
#include <SDL.h>
#include <iostream>

InputEventHandler::InputEventHandler()
{
	exit = a = s = d = w = ml = mr = mlc = mrc = f11 = f11c = false;
	mx = my = 0;
	frameCount = 0;
	secCount = secCount = 0;
	FPS = nullptr;
}

void InputEventHandler::setFps(int* fps)
{
	FPS = fps;
}

void InputEventHandler::updateInputs(int window_width, int window_height)
{
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type) {
		case SDL_QUIT:
			exit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				exit = true;
				break;
			case SDLK_w:
				w = 1;
				break;
			case SDLK_s:
				s = 1;
				break;
			case SDLK_d:
				d = 1;
				break;
			case SDLK_a:
				a = 1;
				break;
			case SDLK_F11:
				f11 = 1;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_w:
				w = 0;
				break;
			case SDLK_s:
				s = 0;
				break;
			case SDLK_d:
				d = 0;
				break;
			case SDLK_a:
				a = 0;
				break;
			case SDLK_F11:
				f11 = 0;
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (((SDL_MouseButtonEvent*)&event)->button) {
			case SDL_BUTTON_LEFT:
				ml = 1;
				break;
			case SDL_BUTTON_RIGHT:
				mr = 1;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (((SDL_MouseButtonEvent*)&event)->button) {
			case SDL_BUTTON_LEFT:
				ml = 0;
				break;
			case SDL_BUTTON_RIGHT:
				mr = 0;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			//mx = event.motion.x * ((float)WINDOW_WIDTH / window_width);
			//my = event.motion.y * ((float)WINDOW_HEIGHT / window_height);
			mx = event.motion.x;
			my = event.motion.y;
		}
	}
	updateClicks(ml, mlc, 0);
	updateClicks(mr, mrc, 1);
	updateClicks(f11, f11c, 2);

	frameCount++;
	secCount = (double)frameCount / *FPS;

	//updates waitMap, ereases elements that finished counting
	for (auto it = waitMap.begin(); it != waitMap.end(); it++) {
		if (it->second >= it->first.first * *FPS) {
			waitMap.erase(it->first);
			break;
		}
		it->second++;
	}
}

bool InputEventHandler::wait(double sec, int specialKey)
{
	// starts at 0 if first time used
	if (waitMap.find({ sec, specialKey }) == waitMap.end()) {
		waitMap[{ sec, specialKey}] = 0;
	}
	// check if seconds passed
	if (waitMap[{ sec, specialKey}] >= sec * *FPS) {
		return true;
	}
	return false;
}

void InputEventHandler::updateClicks(bool a, bool& ac, int type)
{
	if (type >= u.size()) {
		int size = u.size();
		for (int i = 0; i <= type - size; i++) {
			u.push_back(false);
		}
	}

	if (a && !ac && !u[type]) {
		ac = u[type] = true;
	}
	else if (ac && u[type]) {
		ac = false;
	}
	else if (!a && u[type]) {
		ac = u[type] = false;
	}
}
