#include "GameEngine.h"
#include "WindowRenderer.h"
#include "InputEventHandler.h"
#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include <math.h>
#include <string>
#include "Bullet.h"
#include "Hitbox.h"
#include "ObjectType.h"

GameEngine::GameEngine(int* fps)
{
	gameRunning = true;
	ORDER = 0;
	FPS = fps;
	keyInput.setFps(fps);
	DELTA = 60 / (double)(*FPS);
}

void GameEngine::Update()
{
	DELTA = 60 / (double)(*FPS);
	keyInput.updateInputs();
	RunInputs();
	Run();
	UpdateSprites();
	UpdateHitboxes();
	window.Present();
}

void GameEngine::Run()
{
	switch (ORDER) {
	case 0:
		window.SetBackgroundColor(255, 255, 255, 255);
		spriteMap["Player"] = new Sprite("Player", 1280 / 2, 720 / 2, "res/B_Player.png", true);
		spriteMap["Player"]->s = 5;
		spriteMap["Player"]->setAnimator(50, 50, { {5,5} }, { 8 });
		Load(spriteMap["Player"]);
		ORDER++;
		break;
	case 1:
		window.SetBackgroundColor(255, 255, 255, 255);
		Control(spriteMap["Player"], 5);
		Clone(spriteMap["Player"]);
		spriteMap["Player"]->loopAniWhen(0, keyInput.wait(0.1, 0));
		break;
	}
}

bool GameEngine::IsRunning()
{
	return gameRunning;
}

void GameEngine::UpdateSprites()
{
	for (auto it = spriteMap.begin(); it != spriteMap.end(); it++) {
		it->second->Update();
		if (it->second->visible) {
			if (!window.FindLoaded(it->second->img)) {
				Load(it->second);
				std::cout << "Loaded " << it->second->img << std::endl;
			}
			Draw(it->second);
		}
	}
}

void GameEngine::UpdateHitboxes()
{
	for (auto it = hitboxMap.begin(); it != hitboxMap.end(); it++) {
		it->second->Update();
	}
}

void GameEngine::Load(Sprite* sprite)
{
	window.LoadTextureFromFile(sprite->img, sprite->s);
}

void GameEngine::Unload(Sprite* sprite)
{
	window.UnloadTexture(sprite->img);
}

void GameEngine::Draw(Sprite* sprite)
{
	std::pair<std::pair<int, int>, std::pair<int, int>> sheet = sprite->curAni();
	window.DrawTexture(sprite->img, sprite->x, sprite->y, sheet.first.first, sheet.first.second, sheet.second.first, sheet.second.second, sprite->a, sprite->s, SDL_FLIP_NONE);
}

void GameEngine::Draw(Sprite* sprite, int order, int count)
{
	std::pair<std::pair<int, int>, std::pair<int, int>> sheet = sprite->getSheet(order, count);
	window.DrawTexture(sprite->img, sprite->x, sprite->y, sheet.first.first, sheet.first.second, sheet.second.first, sheet.second.second, sprite->a, sprite->s, SDL_FLIP_NONE);
}

void GameEngine::RunInputs()
{
	if (keyInput.exit) {
		gameRunning = false;
	}
}

void GameEngine::Control(Sprite* sprite, double speed)
{
	speed = speed * DELTA;
	int vect[2] = { 0,0 };
	if (keyInput.w) {
		vect[1] -= 1;
	}
	if (keyInput.a) {
		vect[0] -= 1;
	}
	if (keyInput.s) {
		vect[1] += 1;
	}
	if (keyInput.d) {
		vect[0] += 1;
	}
	if (vect[0] != 0 || vect[1] != 0)
	{
		std::pair<double, double> v = UnitVect(speed, vect[0], vect[1]);
		sprite->chanPos(v.first, v.second);
	}
}

void GameEngine::Clone(Sprite* sprite)
{
	std::string name = sprite->name + std::to_string(spriteMap.size());
	spriteMap[name] = new Sprite(name, *sprite);
}

void GameEngine::ShootFromWith(Sprite* sprite, Bullet* bullet, double xpos, double ypos, double speed)
{
	std::string name = "bullet" + std::to_string(spriteMap.size());
	spriteMap[name] = new Bullet(name, sprite->x, sprite->y, bullet->img, true, UnitVect(speed, xpos - sprite->x, ypos - sprite->y), bullet->type);
	hitboxMap[name] = new Hitbox(5 * 5, 5 * 5, spriteMap[name], 0, 0);
}

bool GameEngine::Collision(objectType t1, objectType t2)
{
	for (auto it = hitboxMap.begin(); it != hitboxMap.end(); it++) {
		for (auto it2 = it; it2 != hitboxMap.end(); it2++) {
			if (it != it2) {
				if ((it->second->type == t1 && it2->second->type == t2) || (it->second->type == t2 && it2->second->type == t1)) {
					if (it->second->IsOn(it2->second)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

std::pair<double, double> GameEngine::UnitVect(double speed, double x, double y)
{
	std::pair<double, double> vect;
	vect.first = speed * x / sqrt(y * y + x * x);
	vect.second = speed * y / sqrt(y * y + x * x);
	return vect;
}

GameEngine::~GameEngine()
{
	for (auto it = spriteMap.begin(); it != spriteMap.end(); it++) {
		delete it->second;
	}
	for (auto it = hitboxMap.begin(); it != hitboxMap.end(); it++) {
		delete it->second;
	}
}
