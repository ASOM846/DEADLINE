#pragma once

#include "entity/zombie.hpp"
#include "levelMap.hpp"
#include <algorithm>
#include <cstdlib>
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

	float currentZombieHp;

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

		totalZombiesInWave =
			10 + (wave * 2) + static_cast<int>(std::pow(wave, 1.5));
		currentZombieHp = 100.0f + (wave * 20.0f) * (1.0f + (wave * 0.05));

		spawnInterval = 1.5f * std::pow(0.9, wave - 1);
		spawnInterval = std::max(spawnInterval, 0.25f);

		MAX_ZOMBIES = std::min(15 + wave * 2, 60);
	}

  private:
	void SpawnZombie(std::vector<Zombie> &zombies, const LevelMap &map) {
		int idx = GetRandomValue(0, map.zombieSpawners.size() - 1);
		int tileIndex = map.zombieSpawners[idx];

		int x = (tileIndex % map.width) * map.cellSize;
		int y = (tileIndex / map.width) * map.cellSize;

		float baseSpeed = 2.5 + (wave * 0.05f);
		float randomMod = GetRandomValue(-50, 50) / 100.0F;

		baseSpeed += randomMod;

		zombies.push_back({
			.position = {static_cast<float>(x), static_cast<float>(y)},
			.speed = baseSpeed,
			.hp = static_cast<int>(currentZombieHp),
		});
	}
};
