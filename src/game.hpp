#pragma once

#include "cameraManager.hpp"
#include "collisionManager.hpp"
#include "entity/bullet.hpp"
#include "entity/player.hpp"
#include "entity/zombie.hpp"
#include "levelMap.hpp"
#include "ui.hpp"
#include "waveManager.hpp"
#include <raylib.h>
#include <vector>

class Game {
  public:
	Game() = default;
	~Game() = default;

	void Init();
	void Reset();

	void Update();
	void Render();

  private:
	Player player;

	WaveManager waveManager;

	CameraManager cameraManager;

	UI ui;
	BulletManager bulletManager;
	ZombieManager zombieManager;
	CollisionManager collisionManager;

	LevelMap levelMap;

	std::vector<Zombie> zombies;
	std::vector<Bullet> bullets;
};
