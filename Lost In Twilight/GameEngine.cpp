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
	DrawAll();
	window.Present();
}

void GameEngine::Run()
{
	switch (ORDER) {
	case 0:
		window.SetBackgroundColor(0, 0, 0, 255);

		Level1SetUp();

		ORDER++;
		break;
	case 1:
		window.SetBackgroundColor(0, 0, 0, 255);

		if (keyInput.mlc) {
			HM[ShootFromWith(SM["C_Player"], dynamic_cast<Bullet*>(SM["B_Player"]), keyInput.mx, keyInput.my, 10)];
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
		if (!it->second->unused) {
			switch (it->second->type) {
			case C_ENEMY:

				break;
			case B_PLAYER:
				AngleSpriteToVelo(it->second);
				break;
			}

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
				case C_ENEMY:
					it->second->nextAnimationWhen(keyInput.wait(0.2, 1));
					break;
				case B_PLAYER:
					it->second->nextAnimationWhen(keyInput.wait(0.2, 2));
					break;
				}
			}
		}
	}
}

void GameEngine::UpdateHitboxes()
{
	std::vector<std::string> deleteList;

	for (auto it = HM.begin(); it != HM.end(); it++) {
		it->second->Update();

		if (!it->second->sprite->unused) {
			switch (it->second->type) {
			case B_ENEMY:
			case B_PLAYER:
				//check if bullet hit wall
				if (Collision(it->second, WALL)) {
					it->second->sprite->a = LastCollision.second->AngleOut(it->second);
					it->second->sprite->v = SetVectToAngle(it->second->sprite->a, it->second->sprite->v);
					dynamic_cast<Bullet*>(it->second->sprite)->bounces++;
				}
				if (dynamic_cast<Bullet*>(it->second->sprite)->bounces >= 2) {
					deleteList.push_back(it->second->sprite->name);
				}
				break;
			case C_ENEMY:
				StopAt(it->second, WALL, SCALE * 1);


				if (Collision(it->second, BULLET)) {
					deleteList.push_back(LastCollision.second->sprite->name);
					//deleteList.push_back(it->second->sprite->name);
				}
			case C_PLAYER:
				StopAt(it->second, WALL, SCALE * 1);

				if (Collision(it->second, BULLET)) {
					deleteList.push_back(LastCollision.second->sprite->name);
					B["Reset"] = true;
				}
				break;
			}
		}
	}

	//delete everthing in delete list
	for (int i = 0; i < deleteList.size(); i++) {
		Delete(deleteList[i]);
	}
}

void GameEngine::UpdateWindow()
{
	if (keyInput.f11c) {
		window.ToggleFullscreen();
	}
}

void GameEngine::DrawAll()
{
	for (auto it = SM.begin(); it != SM.end(); it++) {
		if (it->second->visible) {
			Draw(it->second);
		}
	}
	for (auto it = HM.begin(); it != HM.end(); it++) {
		if (B["viewHitboxes"]) {
			if (it->second->isInverse) {
				window.ViewInverseHitbox(it->second->x, it->second->y, it->second->w, it->second->h);
			}
			else {
				window.ViewHitbox(it->second->x, it->second->y, it->second->w, it->second->h);
			}
		}
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

void GameEngine::Delete(std::string name)
{
	delete SM[name];
	SM.erase(name);

	delete HM[name];
	HM.erase(name);
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
	while (SM.find(name) != SM.end()) {
		name = name + '/';
	}
	SM[name] = new Sprite(name, *sprite);

	if (HM.find(sprite->name) != HM.end()) {
		HM[name] = new Hitbox(*HM[sprite->name], sprite);
	}
}

void GameEngine::Clone(Sprite* sprite, int xpos, int ypos, bool show)
{
	std::string name = sprite->name + std::to_string(SM.size());
	while (SM.find(name) != SM.end()) {
		name = name + '/';
	}
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
	while (SM.find(name) != SM.end()) {
		name = name + '/';
	}
	SM[name] = new Bullet(name, *bullet, sprite->x, sprite->y, UnitVect(speed, xpos - sprite->x, ypos - sprite->y));
	HM[name] = new Hitbox(*HM[bullet->name], SM[name]);
	SM[name]->visible = true;

	//activate bullet once it leaves player hitbox
	while (HM[name]->IsOn(HM[sprite->name])) {
		SM[name]->Update(DELTA);
		HM[name]->Update();
	}

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
	if (x == 0 && y == 0) {
		return { 0,0 };
	}

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

bool GameEngine::LineIntersection(std::pair<std::pair<int, int>, std::pair<int, int>> l1, std::pair<std::pair<int, int>, std::pair<int, int>> l2)
{
	double a1x = l1.first.first;
	double a1y = l1.first.second;
	double a2x = l1.second.first;
	double a2y = l1.second.second;
	double b1x = l2.first.first;
	double b1y = l2.first.second;
	double b2x = l2.second.first;
	double b2y = l2.second.second;

	double c1x, c1y, c2x, c2y;
	
	if (b1x < b2x) {
		c1x = b1x;
		c1y = b1y;
		c2x = b2x;
		c2y = b2y;
	}
	else {
		c1x = b2x;
		c1y = b2y;
		c2x = b1x;
		c2y = b1y;
	}

	b1x = c1x; b1y = c1y; b2x = c2x; b2y = c2y;

	if (a1x < a2x) {
		c1x = a1x;
		c1y = a1y;
		c2x = a2x;
		c2y = a2y;
	}
	else {
		c1x = a2x;
		c1y = a2y;
		c2x = a1x;
		c2y = a1y;
	}

	a1x = c1x; a1y = c1y; a2x = c2x; a2y = c2y;

	double am, bm;
	double x;
	double y;
	
	if (a2x == a1x) {
		if (b2x == b1x) {
			if (a1x == b1x) {
				if (a1y > b1y) {
					if ((a1y < a2y ? a1y : a2y) <= (b1y > b2y ? b1y : b2y)) {
						return true;
					}
					else {
						return false;
					}
				}
				else {
					if ((a1y > a2y ? a1y : a2y) >= (b1y < b2y ? b1y : b2y)) {
						return true;
					}
					else {
						return false;
					}
				}
			}
			else {
				return false;
			}
		}
		else {
			x = a1x;
			bm = (b2y - b1y) / (b2x - b1x);
			y = bm * (x - b1x) + b1y;

			if ((a1y < a2y ? a1y : a2y) <= y && (a1y > a2y ? a1y : a2y) >= y && (x >= b1x && x <= b2x)) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (b2x == b1x){
		x = b1x;
		am = (a2y - a1y) / (a2x - a1x);
		y = am * (x - a1x) + a1y;

		if ((b1y < b2y ? b1y : b2y) <= y && (b1y > b2y ? b1y : b2y) >= y && (x >= a1x && x <= a2x)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		am = (a2y - a1y) / (a2x - a1x);
		bm = (b2y - b1y) / (b2x - b1x);

		if (am == bm) {
			if (b1y - a1y == am * (b1x - a1x)) {
				return true;
			}
			else {
				false;
			}
		}

		x = (am * a1x - a1y - bm * b1x + b1y) / (am - bm);
	}

	if (x >= a1x && x <= a2x && x >= b1x && x <= b2x) {
		return true;
	}
	return false;
}

bool GameEngine::HitboxBetweenHitboxes(Hitbox* s, Hitbox* t)
{
	double sx = s->sprite->x;
	double sy = s->sprite->y;
	double tx = t->sprite->x;
	double ty = t->sprite->y;

	//ignores bullet hitboxes
	for (auto it = HM.begin(); it != HM.end(); it++) {
		if (it->second->type != BULLET && it->second != s && it->second != t && it->second->active) {
			for (int i = 0; i < 4; i++) {
				if (LineIntersection({ it->second->p[i % 4], it->second->p[(i + 1) % 4] }, { {sx,sy},{tx,ty} })) {
					return true;
				}
			}
		}
	}

	return false;
}

bool GameEngine::HitboxBetweenHitboxesIgnore(Hitbox* s, Hitbox* t, Hitbox* Wall)
{
	double sx = s->sprite->x;
	double sy = s->sprite->y;
	double tx = t->sprite->x;
	double ty = t->sprite->y;

	//ignores bullet hitboxes
	for (auto it = HM.begin(); it != HM.end(); it++) {
		if (it->second->type != BULLET && it->second != s && it->second != t && it->second->active) {
			for (int i = 0; i < 4; i++) {
				if (it->second != Wall) {
					if (LineIntersection({ it->second->p[i % 4], it->second->p[(i + 1) % 4] }, { {sx,sy},{tx,ty} })) {
						/*std::cout << "(" << sx << "," << sy << ")" << "(" << tx << "," << ty << ")"
							<< "(" << it->second->p[i % 4].first << "," << it->second->p[i % 4].second << ")"
							<< "(" << it->second->p[(i + 1) % 4].first << "," << it->second->p[(i + 1) % 4].second << ")";*/

						//std::cout << it->second->sprite->name << " ";
						return true;
					}
				}
			}
		}
	}

	return false;
}

Hitbox* GameEngine::ImaginaryHitbox(Hitbox* h, int x, int y)
{
	std::string name = h->sprite->name + 'I';
	while (SM.find(name) != SM.end()) {
		name = name + 'I';
	}
	SM[name] = new Sprite(name, x, y, "NO_FILE", false);
	SM[name]->unused = true;
	HM[name] = new Hitbox(*h, SM[name]);
	return HM[name];
}

int GameEngine::CheckWallBounce(Hitbox* shooter, Hitbox* target)
{
	shooter = HM["C_Player"];
	target = HM["C_Enemy"];
	Hitbox* wall;
	int count = 0;

	//check all walls;
	for (auto it = HM.begin(); it != HM.end(); it++) {
		if (it->second->type == WALL) {
			wall = it->second;

			//check all sides;
			for (int i = 0; i < 4; i++) {
				if (CheckWallSideBounce(wall, i, shooter, target)) {
					count++;
				}
			}
		}
	}
	
	return count;
}

bool GameEngine::CheckWallSideBounce(Hitbox* wall, int side, Hitbox* s, Hitbox* t)
{
	bool ret = false;
	std::vector<Hitbox*> ih;
	std::vector<Hitbox*> createToImaginary;
	Hitbox* iht = t;

	for (auto it = HM.begin(); it != HM.end(); it++) {
		if (it->second->type != BULLET) {
			//only create imaginary hitboxes to the same side as shooter/start
			if (side % 2) {
				if ((it->second->sprite->x >= wall->p[side].first) == (s->sprite->x >= wall->p[side].first)) {
					createToImaginary.push_back(it->second);
				}
			}
			else {
				if ((it->second->sprite->y >= wall->p[side].second) == (s->sprite->y >= wall->p[side].second)) {
					createToImaginary.push_back(it->second);
				}
			}
		}
	}

	for (int i = 0; i < createToImaginary.size(); i++) {
		if (side % 2) {
			ih.push_back(ImaginaryHitbox(createToImaginary[i], createToImaginary[i]->sprite->x + (wall->p[side].first - createToImaginary[i]->sprite->x) * 2, createToImaginary[i]->sprite->y));
		}
		else {
			ih.push_back(ImaginaryHitbox(createToImaginary[i], createToImaginary[i]->sprite->x, createToImaginary[i]->sprite->y + (wall->p[side].second - createToImaginary[i]->sprite->y) * 2));
		}
		if (createToImaginary[i] == t) {
			iht = ih[ih.size() - 1];
		}
	}


	//view imaginary hitboxes
	/*for (auto it = HM.begin(); it != HM.end(); it++) {
		if (B["viewHitboxes"]) {
			if (it->second->isInverse) {
				window.ViewInverseHitbox(it->second->x, it->second->y, it->second->w, it->second->h);
			}
			else {
				window.ViewHitbox(it->second->x, it->second->y, it->second->w, it->second->h);
			}
		}
	}*/

	//check if aim reflects off actual wall
	if (LineIntersection({ { s->sprite->x ,s->sprite->y }, { iht->sprite->x ,iht->sprite->y } }, { wall->p[side], wall->p[(side + 1) % 4] })) {
		if (!HitboxBetweenHitboxesIgnore(s, iht, wall)) {
			//std::cout << "Wall: " << wall->sprite->name << " Side: " << side << "|";
			window.ViewHitbox(iht->x, iht->y, iht->w, iht->h);
			ret = true;
		}
	}
	

	for (int i = 0; i < ih.size(); i++) {
		Delete(ih[i]->sprite->name);
	}

	return ret;
}

void GameEngine::StopAt(Hitbox* h, objectType t, double distanceFromType)
{
	if (CollisionExtended(h, t, distanceFromType)) {
		double xv = h->sprite->v.first;
		double yv = h->sprite->v.second;

		h->sprite->v = { 0, -yv }; //check x
		h->sprite->move(DELTA);
		h->Update();
		if (CollisionExtended(h, t, distanceFromType)) {
			h->sprite->v.first = -xv;
		}
		h->sprite->v.second = yv; //check y
		h->sprite->move(DELTA);
		h->Update();
		if (CollisionExtended(h, t, distanceFromType)) {
			h->sprite->v = { 0, -yv };
			h->sprite->move(DELTA);
			h->Update();
		}
	}
}

void GameEngine::Level1SetUp()
{
	SM["C_Player"] = new Sprite("C_Player", 1920 / 2, 1080 / 2, "res/C_Player.png", true, C_PLAYER, SCALE);
	SM["C_Player"]->setAnimator(62, 62, { {8,8} }, { 7 });
	SM["C_Player"]->setAnimationOrder(0);
	HM["C_Player"] = new Hitbox(6, 6, SM["C_Player"], 0, 0);

	SM["B_Player"] = new Bullet("B_Player", 0, 0, "res/B_Player.png", false, B_PLAYER, SCALE);
	SM["B_Player"]->unused = true;
	SM["B_Player"]->setAnimator(35, 35, { {3,3} }, { 18 });
	SM["B_Player"]->setAnimationOrder(0);
	SM["B_Player"]->angleOffset = 45;
	HM["B_Player"] = new Hitbox(1, 1, SM["B_Player"], 1, -1);

	SM["W_Edge1"] = new Sprite("W_Edge1", 1920 / 2, (SCALE * 3), "res/W_Test.png", true, W_EDGE, SCALE);
	SM["W_Edge1"]->setAnimator(128, 6, { { 128,6 } }, { 1 });
	HM["W_Edge1"] = new Hitbox(128, 6, SM["W_Edge1"],0,0);

	SM["W_Edge2"] = new Sprite("W_Edge2", 1920 - (SCALE * 3), 1080 / 2, "res/W_Test.png", true, W_EDGE, SCALE);
	SM["W_Edge2"]->setAnimator(6, 72, { { 6, 72 } }, { 1 });
	HM["W_Edge2"] = new Hitbox(6, 72, SM["W_Edge2"], 0, 0);

	SM["W_Edge3"] = new Sprite("W_Edge3", 1920 / 2, 1080 - (SCALE * 3), "res/W_Test.png", true, W_EDGE, SCALE);
	SM["W_Edge3"]->setAnimator(128, 6, { { 128,6 } }, { 1 });
	HM["W_Edge3"] = new Hitbox(128, 6, SM["W_Edge3"], 0, 0);

	SM["W_Edge4"] = new Sprite("W_Edge4", (SCALE * 3), 1080 / 2, "res/W_Test.png", true, W_EDGE, SCALE);
	SM["W_Edge4"]->setAnimator(6, 72, { { 6, 72 } }, { 1 });
	HM["W_Edge4"] = new Hitbox(6, 72, SM["W_Edge4"], 0, 0);

	SM["W_Test"] = new Sprite("W_Test", 1920 / 2 - 250, 1080 / 2, "res/W_Test.png", true, WALL);
	SM["W_Test"]->setAnimator(10, 30, { { 10,30 } }, { 1 });
	SM["W_Test"]->s = 15;
	HM["W_Test"] = new Hitbox(10, 30, SM["W_Test"], 0, 0);

	SM["C_Enemy"] = new Sprite("C_Enemy", 1920 / 4 * 3, 1080 / 4, "res/C_Enemy.png", true, C_ENEMY, SCALE);
	SM["C_Enemy"]->setAnimator(62, 62, { {8,8} }, { 7 });
	SM["C_Enemy"]->setAnimationOrder(0);
	HM["C_Enemy"] = new Hitbox(6, 6, SM["C_Enemy"], 0, 0);

	SM["B_Enemy"] = new Bullet("B_Enemy", 0, 0, "res/B_Enemy.png", false, B_ENEMY, SCALE);
	SM["B_Enemy"]->unused = true;
	SM["B_Enemy"]->setAnimator(35, 35, { {3,3} }, { 18 });
	SM["B_Enemy"]->setAnimationOrder(0);
	SM["B_Enemy"]->angleOffset = 45;
	HM["B_Enemy"] = new Hitbox(1, 1, SM["B_Enemy"], 1, -1);
}

void GameEngine::Control(Sprite* sprite, double speed)
{
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
	std::pair<double, double> v = UnitVect(speed, vect[0], vect[1]);
	sprite->v = v;
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
