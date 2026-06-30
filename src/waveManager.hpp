#pragma once

#include "entity/zombie.hpp"
#include "levelMap.hpp"
#include <raylib.h>
#include <vector>
class WaveManager {
  public:
	int wave = 1;
	float spawnTimer = 0.0F;

	void Update(std::vector<Zombie> &zombies, const LevelMap &map) {
		spawnTimer -= GetFrameTime();

		if (spawnTimer <= 0.0F) {
			SpawnZombie(zombies, map);
			spawnTimer = 2.0F;
		}
	}

  private:
	static void SpawnZombie(std::vector<Zombie> &zombies, const LevelMap &map) {
		int idx = GetRandomValue(0, map.zombieSpawners.size() - 1);
		int tileIndex = map.zombieSpawners[idx];

		int x = (tileIndex % map.width) * map.cellSize;
		int y = (tileIndex / map.width) * map.cellSize;

		zombies.push_back(Zombie{
			.position = Vector2{static_cast<float>(x), static_cast<float>(y)}});
	}
};
