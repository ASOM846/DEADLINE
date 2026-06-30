#pragma once

#include "entity/player.hpp"
#include "entity/zombie.hpp"
#include "levelMap.hpp"
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

	ZombieManager zombieManager;

	LevelMap levelMap;

	std::vector<Zombie> zombies;

	Camera2D camera;
};
