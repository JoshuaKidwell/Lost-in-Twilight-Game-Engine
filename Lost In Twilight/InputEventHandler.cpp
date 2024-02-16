#include "InputEventHandler.h"
#include <SDL.h>
#include <iostream>

InputEventHandler::InputEventHandler()
{
	exit = a = s = d = w = ml = mr = mlc = mrc = false;
	mx = my = 0;
	umlc = umrc = false;
	frameCount = 0;
	secCount = secCount = 0;
	FPS = nullptr;
}

void InputEventHandler::setFps(int* fps)
{
	FPS = fps;
}

void InputEventHandler::updateInputs()
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
		}
	}
	updateClicks(ml, mlc, umlc);
	updateClicks(mr, mrc, umrc);
	SDL_GetMouseState(&mx, &my);
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

void InputEventHandler::updateClicks(bool a, bool& ac, bool& uac)
{
	if (a && !ac && !uac) {
		ac = uac = true;
	}
	else if (ac && uac) {
		ac = false;
	}
	else if (!a && uac) {
		ac = uac = false;
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
