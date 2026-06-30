#pragma once

#include "entity/zombie.hpp"
#include "levelMap.hpp"
#include <algorithm>
#include <raylib.h>
#include <vector>

class WaveManager {
  public:
	int wave = 1;
	float spawnTimer = 0.0F;
	float spawnInterval = 2.0F;

	int spawnedZombies = 0;
	int totalZombiesInWave = 0;

	int MAX_ZOMBIES = 25;

	bool isIntermission = false;
	float intermissionTimer = 0.0f;
	const float intermissionDuration = 7.0f;

	void Update(std::vector<Zombie> &zombies, const LevelMap &map) {
		if (isIntermission) {
			intermissionTimer -= GetFrameTime();
			if (intermissionTimer <= 0.0f) {
				isIntermission = false;
				wave++;
				StartNextWave();
			}
			return;
		}
		spawnTimer += GetFrameTime();

		if (spawnTimer >= spawnInterval && zombies.size() < MAX_ZOMBIES &&
			spawnedZombies < totalZombiesInWave) {
			SpawnZombie(zombies, map);
			spawnedZombies++;
			spawnTimer = 0.0f;
		}

		if (spawnedZombies == totalZombiesInWave && zombies.empty()) {
			isIntermission = true;
			intermissionTimer = intermissionDuration;
		}
	}

	void StartNextWave() {
		spawnedZombies = 0;

		if (wave <= 5) {
			totalZombiesInWave = 6 + (wave * 4);
		} else {
			totalZombiesInWave = 24 + (wave * 6);
		}

		spawnInterval = 2.0f - (wave * 0.05);

		spawnInterval = std::max(spawnInterval, 0.3f);
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
