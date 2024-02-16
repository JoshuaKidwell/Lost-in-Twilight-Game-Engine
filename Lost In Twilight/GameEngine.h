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
	GameEngine();
	void Update();
	void Run();
	bool IsRunning();
	void UpdateSprites();
	void UpdateHitboxes();
	void LoadSprite(Sprite*);
	void UnloadSprite(Sprite*);
	void DrawSprite(Sprite*);
	void DrawSprite(Sprite*, int order, int count);
	void RunInputs();
	void ControlSprite(Sprite* sprite, double speed);
	void ShootFromWith(Sprite* sprite, Bullet* bullet, double, double, double speed);
	bool Collision(objectType, objectType);
	std::pair<double, double> UnitVect(double speed, double x, double y);
	~GameEngine();
private:
	bool gameRunning;
	int Order;
	InputEventHandler keyInput;
	WindowRenderer window;
	std::map<std::string, Sprite*> spriteMap;
	std::map<std::string, Hitbox*> hitboxMap;
};

#endif

