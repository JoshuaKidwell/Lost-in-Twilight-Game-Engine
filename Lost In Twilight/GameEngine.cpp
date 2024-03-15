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
	SCALE = 15;
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
	Commands();
	UpdateSprites();
	UpdateHitboxes();
	UpdateWindow();
	window.Present();
}

void GameEngine::Run()
{
	switch (ORDER) {
	case 0:
		window.SetBackgroundColor(0, 0, 0, 255);

		SM["C_Player"] = new Sprite("C_Player", 1920 / 2, 1080 / 2, "res/C_Player.png", true, C_PLAYER, SCALE);
		SM["C_Player"]->setAnimator(62, 62, { {8,8} }, { 7 });
		SM["C_Player"]->setAnimationOrder(0);
		HM["C_Player"] = new Hitbox(6, 6, SM["C_Player"], 0, 0);

		SM["B_Player"] = new Bullet("B_Player", -100, -100, "res/B_Player.png", false, B_PLAYER, SCALE);
		SM["B_Player"]->setAnimator(35, 35, { {3,3} }, { 18 });
		SM["B_Player"]->setAnimationOrder(0);
		SM["B_Player"]->angleOffset = 45;
		HM["B_Player"] = new Hitbox(1, 1, SM["B_Player"], 1, -1);

		SM["W_Edge"] = new Sprite("W_Edge", 1920 / 2, 1080 / 2, "res/W_Edge.png", true, W_EDGE);
		SM["W_Edge"]->setAnimator(128, 72, { { 128,72 } }, { 1 });
		SM["W_Edge"]->s = 15;
		HM["W_Edge1"] = new Hitbox(6, 72, SM["W_Edge"], 128 / 2 - 3, 0);
		HM["W_Edge2"] = new Hitbox(128, 6, SM["W_Edge"], 0, 72 / 2 - 3);
		HM["W_Edge3"] = new Hitbox(6, 72, SM["W_Edge"], -128 / 2 + 3, 0);
		HM["W_Edge4"] = new Hitbox(128, 6, SM["W_Edge"], 0, -72 / 2 + 3);

		ORDER++;
		break;
	case 1:
		window.SetBackgroundColor(0, 0, 0, 255);

		if (keyInput.mlc) {
			ShootFromWith(SM["C_Player"], dynamic_cast<Bullet*>(SM["B_Player"]), keyInput.mx, keyInput.my, 10);
		}

		Control(SM["C_Player"], 5);
		
		break;
	}
}

void GameEngine::Commands()
{
	if (keyInput.hc) {
		if (B["viewHitboxes"]) {
			B["viewHitboxes"] = false;
		}
		else {
			B["viewHitboxes"] = true;
		}
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
			case C_PLAYER:
				it->second->nextAnimationWhen(keyInput.wait(0.2, 0));
				break;
			case B_PLAYER:
				AngleSpriteToVelo(it->second);
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

		switch (it->second->type) {
		case B_PLAYER:
			//activate bullet once it leaves player hitbox
			if (!it->second->active) {
				if (!it->second->IsOnExtended(HM["C_Player"], 5)) {
					it->second->active = true;
				}
			}

			//check if bullet hit wall
			if (Collision(it->second, WALL)) {
				it->second->sprite->a = LastCollision.second->AngleOut(it->second);
				it->second->sprite->v = SetVectToAngle(it->second->sprite->a, it->second->sprite->v);
			}
			break;
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
	window.DrawTexture(sprite->img, sprite->x, sprite->y, sheet.first.first, sheet.first.second, sheet.second.first, sheet.second.second, sprite->a + sprite->angleOffset, sprite->s, SDL_FLIP_NONE);
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

void GameEngine::AngleSpriteToVelo(Sprite* sprite)
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
		sprite->a = atan(y / x) * 180/3.1415;
	}
	else {
		sprite->a = atan(y / x) * 180 / 3.1415 + 180;
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
							//set LastCollision to t1, t2
							if ((it->second->type == t1 && it2->second->type == t2)) {
								LastCollision = { it->second, it2->second };
							}
							else {
								LastCollision = { it2->second, it->second };
							}

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
					LastCollision = { h, it->second };
					return true;
				}
			}
		}
	}

	return false;
}

bool GameEngine::CollisionExtended(Hitbox* h, objectType t, int ext)
{
	//Does not check if h is active
	for (auto it = HM.begin(); it != HM.end(); it++) {
		if (it->second != h && it->second->active) {
			if (it->second->type == t) {
				if (it->second->IsOnExtended(h, ext)) {
					LastCollision = { h, it->second };
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
							//set LastCollision to t1, t2
							if ((it->second->type == t1 && it2->second->type == t2)) {
								LastCollision = { it->second, it2->second };
							}
							else {
								LastCollision = { it2->second, it->second };
							}

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

std::pair<double, double> GameEngine::SetVectToAngle(double a, std::pair<double, double> v)
{
	std::pair<double, double> newV;
	double length = sqrt(v.first * v.first + v.second * v.second);
	newV.second = length * sin(a * 3.1415 / 180);
	newV.first = length * cos(a * 3.1415 / 180);
	return newV;
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

		switch (sprite->type) {
		case C_PLAYER:
			sprite->chanPos(v.first, 0);
			HM["C_Player"]->Update();
			if (CollisionExtended(HM["C_Player"], WALL, 1 * sprite->s)) {
				sprite->chanPos(-v.first, 0);
				HM["C_Player"]->Update();
			}
			sprite->chanPos(0, v.second);
			HM["C_Player"]->Update();
			if (CollisionExtended(HM["C_Player"], WALL, 1 * sprite->s)) {
				sprite->chanPos(0, -v.second);
				HM["C_Player"]->Update();
			}
			break;
		}
	}
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
