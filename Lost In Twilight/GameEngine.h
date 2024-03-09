#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "WindowRenderer.h"
#include "InputEventHandler.h"
#include "Sprite.h"
#include <map>
#include <vector>
#include <string>
#include "Bullet.h"
#include "Hitbox.h"

class GameEngine {
public:
	GameEngine(int* fps);
	void Update();
	void Run();
	void Commands();
	bool IsRunning();
	void UpdateSprites();
	void UpdateHitboxes();
	void UpdateWindow();
	void Load(Sprite*);
	void Unload(Sprite*);
	void Draw(Sprite*);
	void Draw(Sprite*, int order, int count);
	void RunInputs();
	/////////////////////////////////////////////////////////////////////////
	void Clone(Sprite*);
	void Clone(Sprite*, int xpos, int ypos, bool show);
	std::string ShootFromWith(Sprite* sprite, Bullet* bullet, double, double, double speed);
	void AngleSpriteToVelo(Sprite* sprite);
	bool Collision(objectType, objectType);
	bool Collision(Hitbox*, objectType);
	bool CollisionExtended(Hitbox*, objectType, int ext);
	bool CollisionIgnore(objectType, objectType, Hitbox*);
	std::pair<double, double> UnitVect(double speed, double x, double y);
	std::pair<double, double> SetVectToAngle(double a, std::pair<double, double> v);
	/////////////////////////////////////////////////////////////////////////
	void Control(Sprite* sprite, double speed);
	void LoadPlayerBullets(int bulletCount);
	~GameEngine();
private:
	bool gameRunning;
	int ORDER;
	int* FPS;
	double DELTA;
	InputEventHandler keyInput;
	WindowRenderer window;
	std::map<std::string, Sprite*> SM;
	std::map<std::string, Hitbox*> HM;
	std::pair<Hitbox*, Hitbox*> LastCollision;

	std::map<std::string, bool> B; //sort of global booleans
	std::map<std::string, int> I; //sort of global integers
	std::map<std::string, std::string> S; //you get the point
};

#endif

