#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "WindowRenderer.h"
#include "InputEventHandler.h"
#include "Sprite.h"
#include <map>
#include <string>
#include "Bullet.h"
#include "Hitbox.h"

class GameEngine {
public:
	GameEngine(int* fps);
	void Update();
	void Run();
	bool IsRunning();
	void UpdateSprites();
	void UpdateHitboxes();
	void UpdateWindow();
	void Load(Sprite*);
	void Unload(Sprite*);
	void Draw(Sprite*);
	void Draw(Sprite*, int order, int count);
	void RunInputs();
	void Control(Sprite* sprite, double speed);
	void Clone(Sprite*);
	void Clone(Sprite*, int xpos, int ypos, bool show);
	void ShootFromWith(Sprite* sprite, Bullet* bullet, double, double, double speed);
	bool Collision(objectType, objectType);
	std::pair<double, double> UnitVect(double speed, double x, double y);
	~GameEngine();
private:
	bool gameRunning;
	int ORDER;
	int* FPS;
	double DELTA;
	InputEventHandler keyInput;
	WindowRenderer window;
	std::map<std::string, Sprite*> spriteMap;
	std::map<std::string, Hitbox*> hitboxMap;
};

#endif

