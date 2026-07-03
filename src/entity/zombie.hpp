#pragma once

#include "../levelMap.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>

struct Zombie {
	Vector2 position;

	int radius{20};
	int speed{3};

	int hp{100};
	int damage{10};
	int value{20};
	bool alive{true};

	void Update(const LevelMap &map) {
		if (hp <= 0) {
			alive = false;
			return;
		}

		int zX = static_cast<int>(position.x) / map.cellSize;
		int zY = static_cast<int>(position.y) / map.cellSize;

		int bestX = zX;
		int bestY = zY;

		int minDistance = map.INF;

		int dx[] = {0, 0, -1, 1};
		int dy[] = {-1, 1, 0, 0};

		for (int i = 0; i < 4; i++) {
			int nx = zX + dx[i];
			int ny = zY + dy[i];

			if (nx >= 0 && nx < map.width && ny >= 0 && ny < map.height) {
				int index = ny * map.width + nx;
				if (map.distanceMap[index] < minDistance) {
					minDistance = map.distanceMap[index];
					bestX = nx;
					bestY = ny;
				}
			}
		}

		Vector2 targetPos;
		targetPos.x = bestX * map.cellSize + map.cellSize / 2;
		targetPos.y = bestY * map.cellSize + map.cellSize / 2;

		float dirX = targetPos.x - position.x;
		float dirY = targetPos.y - position.y;
		float distance = std::sqrt(dirX * dirX + dirY * dirY);

		if (distance > 2.0f) {
			position.x += (dirX / distance) * speed;
			position.y += (dirY / distance) * speed;
		}
	}

	void Render() const { DrawCircleV(position, radius, GREEN); }
};

class ZombieManager {
  public:
	void UpdateAll(std::vector<Zombie> &zombies, const LevelMap &map) {
		for (auto &z : zombies) {
			if (!z.alive)
				continue;
			z.Update(map);
		}

		zombies.erase(std::remove_if(zombies.begin(), zombies.end(),
									 [](const Zombie &z) { return !z.alive; }),
					  zombies.end());
	}

	void ResolveZombieCollision(std::vector<Zombie> &zombies) {
		for (size_t i = 0; i < zombies.size(); i++) {
			for (size_t j = 0; j < zombies.size(); j++) {
				float dx = zombies[j].position.x - zombies[i].position.x;
				float dy = zombies[j].position.y - zombies[i].position.y;
				float distance = std::sqrt(dx * dx + dy * dy);

				float minDistance = zombies[i].radius * 2;

				if (distance < minDistance) {
					if (distance == 0.0f) {
						dx = 1.0f;
						dy = 0.0f;
						distance = 1.0f;
					}

					float overlap = minDistance - distance;

					float nx = dx / distance;
					float ny = dy / distance;

					zombies[i].position.x -= nx * overlap * 0.5;
					zombies[i].position.y -= ny * overlap * 0.5;

					zombies[j].position.x += nx * overlap * 0.5;
					zombies[j].position.y += ny * overlap * 0.5;
				}
			}
		}
	}
};
