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
#include "GlobalVariables.h"

GameEngine::GameEngine(int* fps)
{
	gameRunning = true;
	ORDER = 0;
	FPS = fps;
	keyInput.setFps(fps);
	DELTA = DESIRED_FPS / (double)(*FPS);
}

void GameEngine::Update()
{
	keyInput.updateInputs(window.GetWindowWidth(), window.GetWindowHeight());
	DELTA = DESIRED_FPS / (double)(*FPS);
	RunInputs();
	Run();
	UpdateSprites();
	UpdateHitboxes();
	UpdateWindow();
	window.Present();
}

void GameEngine::Run()
{
	switch (ORDER) {
	case 0:
		//B["viewHitboxes"] = true;

		window.SetBackgroundColor(0, 0, 0, 255);

		SM["F_P"] = new Sprite("F_P", 320 / 2, 240 / 2, "res/F_Player.png", true, F_PLAYER);
		SM["F_P"]->setAnimator(62, 62, { {8,8}, {8,8} }, { 20, 20 });
		SM["F_P"]->setAnimationOrder(1);
		SM["F_P"]->s = 10;
		HM["F_P"] = new Hitbox(6, 6, SM["F_P"], -3, -3);

		SM["B_P"] = new Bullet("B_P", -100, -100, "res/B_Player.png", false, B_PLAYER);
		SM["B_P"]->setAnimator(35, 35, { {3,3} }, { 18 });
		SM["B_P"]->setAnimationOrder(0);
		SM["B_P"]->s = 10;
		HM["B_P"] = new Hitbox(1, 1, SM["B_P"], 0.5, -1.5);

		I["PlayerBullets"] = 4;
		B["PlayerLoad"] = false;

		ORDER++;
		break;
	case 1:
		window.SetBackgroundColor(0, 0, 0, 255);

		Control(SM["F_P"], 5);

		//Loads and shoots bullets for player
		if (keyInput.mlc && I["PlayerBullets"] != 0) {
			HM[ShootFromWith(SM["F_P"], dynamic_cast<Bullet*>(SM["B_P"]), keyInput.mx, keyInput.my, 8)]->active = false;
			I["PlayerBullets"] -= 1;
		}
		if (I["PlayerBullets"] < 4) {
			B["PlayerLoad"] = true;
		}
		else {
			B["PlayerLoad"] = false;
		}

		if (I["LastPlayerFrame"] != SM["F_P"]->getFrameNum()) {
			switch (SM["F_P"]->getFrameNum()) {
			case 0:
			case 5:
			case 10:
			case 15:
				if (B["PlayerLoad"]) {
					SM["F_P"]->setAnimationOrder(0);
					B["PlayerLoading"] = true;
				}
				else {
					SM["F_P"]->setAnimationOrder(1);
					B["PlayerLoading"] = false;
				}
				break;
			case 4:
			case 9:
			case 14:
			case 19:
				if (B["PlayerLoading"]) {
					I["PlayerBullets"]++;
				}
				break;
			}
		}
		I["LastPlayerFrame"] = SM["F_P"]->getFrameNum();


		if (Collision(BULLET, F_PLAYER)) {
			std::cout << "OWWW";
		}

		break;
	}
}

bool GameEngine::IsRunning()
{
	return gameRunning;
}

void GameEngine::UpdateSprites()
{
	for (auto it = SM.begin(); it != SM.end(); it++) {
		it->second->Update(DELTA);
		if (it->second->visible) {
			if (!window.FindLoaded(it->second->img)) {
				Load(it->second);
				std::cout << "Loaded " << it->second->img << std::endl;
			}

			switch (it->second->type) {
			case F_PLAYER:
				it->second->nextAnimationWhen(keyInput.wait(0.1, 0));
				break;
			case B_PLAYER:
				AngleSpriteToVelo(it->second, 45);
				it->second->nextAnimationWhen(keyInput.wait(0.2, 1));
				break;
			}

			Draw(it->second);
		}
	}
}

void GameEngine::UpdateHitboxes()
{
	for (auto it = HM.begin(); it != HM.end(); it++) {
		it->second->Update();

		if (B["viewHitboxes"]) {
			window.ViewHitbox(it->second->x, it->second->y, it->second->w, it->second->h);
		}

		if (it->second->type == B_PLAYER && !it->second->active) {
			if (!it->second->IsOnExtended(HM["F_P"], 5)) {
				it->second->active = true;
			}
		}
	}
}

void GameEngine::UpdateWindow()
{
	if (keyInput.f11c) {
		window.ToggleFullscreen();
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
	std::string name = sprite->name + std::to_string(SM.size());
	SM[name] = new Sprite(name, *sprite);

	if (HM.find(sprite->name) != HM.end()) {
		HM[name] = new Hitbox(*HM[sprite->name], sprite);
	}
}

void GameEngine::Clone(Sprite* sprite, int xpos, int ypos, bool show)
{
	std::string name = sprite->name + std::to_string(SM.size());
	Sprite newSprite(name, *sprite);
	newSprite.x = xpos;
	newSprite.y = ypos;
	newSprite.visible = show;
	SM[name] = new Sprite(name, newSprite);

	if (HM.find(sprite->name) != HM.end()) {
	HM[name] = new Hitbox(*HM[sprite->name], sprite);
	}
}

std::string GameEngine::ShootFromWith(Sprite* sprite, Bullet* bullet, double xpos, double ypos, double speed)
{
	std::string name = bullet->name + std::to_string(SM.size());
	SM[name] = new Bullet(name, *bullet, sprite->x, sprite->y, UnitVect(speed, xpos - sprite->x, ypos - sprite->y));
	HM[name] = new Hitbox(*HM[bullet->name], SM[name]);
	SM[name]->visible = true;

	return name;
}

void GameEngine::AngleSpriteToVelo(Sprite* sprite, double chanA)
{
	double x = sprite->v.first;
	double y = sprite->v.second;
	if (x == 0) {
		if (y >= 0) {
			sprite->a = -90;
		}
		else {
			sprite->a = 90;
		}
	}
	else if (x > 0) {
		sprite->a = atan(y / x) * 180/3.1415 + chanA;
	}
	else {
		sprite->a = atan(y / x) * 180 / 3.1415 + chanA + 180;
	}
}

bool GameEngine::Collision(objectType t1, objectType t2)
{
	for (auto it = HM.begin(); it != HM.end(); it++) {
		if (it->second->active) {
			for (auto it2 = it; it2 != HM.end(); it2++) {
				if (it != it2 && it2->second->active) {
					if ((it->second->type == t1 && it2->second->type == t2) || (it->second->type == t2 && it2->second->type == t1)) {
						if (it->second->IsOn(it2->second)) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool GameEngine::Collision(Hitbox* h, objectType t)
{
	//Does not check if h is active
	for (auto it = HM.begin(); it != HM.end(); it++) {
		if (it->second != h && it->second->active) {
			if (it->second->type == t) {
				if (it->second->IsOn(h)) {
					return true;
				}
			}
		}
	}

	return false;
}

bool GameEngine::CollisionIgnore(objectType t1, objectType t2, Hitbox* hit)
{
	for (auto it = HM.begin(); it != HM.end(); it++) {
		//ignore if = hitbox
		if (it->second != hit && it->second->active) {
			for (auto it2 = it; it2 != HM.end(); it2++) {
				if (it != it2 && it2->second->active) {
					if ((it->second->type == t1 && it2->second->type == t2) || (it->second->type == t2 && it2->second->type == t1)) {
						if (it->second->IsOn(it2->second)) {
							return true;
						}
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
	for (auto it = SM.begin(); it != SM.end(); it++) {
		delete it->second;
	}
	for (auto it = HM.begin(); it != HM.end(); it++) {
		delete it->second;
	}
}
