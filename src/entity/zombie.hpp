#pragma once

#include "../levelMap.hpp"
#include "../textureManager.hpp"
#include "../utils.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>

struct Zombie {
	Vector2 position;

	int radius{20};
	float visualRadius{25};
	float wallRadius{radius * 0.8f};

	float speed{3};

	Vector2 pathfinfingOffset{0, 0};

	int hp;
	int damage{10};
	int damageTaken{0};

	int value{10};
	bool alive{true};

	int dropChance{10}; // from 0% to 100%

	float rotation{0.0f};
	float textureRotation{90.0f};

	void InitializeRandomTrails() {
		pathfinfingOffset.x = static_cast<float>(GetRandomValue(-15, 15));
		pathfinfingOffset.y = static_cast<float>(GetRandomValue(-15, 15));
	}

	void Update(const LevelMap &map, Vector2 playerPos) {
		if (hp <= 0) {
			alive = false;
			return;
		}

		Vector2 targetPos;

		if (CheckLineOfSight(position, playerPos, map)) {
			targetPos = playerPos;
		} else {
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

			targetPos.x =
				bestX * map.cellSize + (map.cellSize / 2) + pathfinfingOffset.x;
			targetPos.y =
				bestY * map.cellSize + (map.cellSize / 2) + pathfinfingOffset.y;
		}
		float dirX = targetPos.x - position.x;
		float dirY = targetPos.y - position.y;
		float distance = std::sqrt(dirX * dirX + dirY * dirY);

		if (distance > 2.0f) {
			rotation = std::atan2(dirY, dirX) * 57.29578f;

			position.x += (dirX / distance) * speed;
			position.y += (dirY / distance) * speed;
		}
	}

	void Render(TextureManager &tm) const {

		// DrawCircleV(position, radius, GREEN);

		float localOffsetY = 0.0f;

		float textureScale = 1.3f;

		float drawWidth = visualRadius * 2.0f * textureScale;
		float drawHeight = visualRadius * 2.0f * textureScale;

		Texture2D zombieTex = tm.get(TextureId::ZOMBIE);

		Rectangle src = {0, 0, static_cast<float>(zombieTex.width),
						 static_cast<float>(zombieTex.height)};

		Rectangle dstCorrected = {position.x, position.y, drawWidth,
								  drawHeight};

		Vector2 origin = {drawWidth / 2.0f, (drawHeight / 2.0f) + localOffsetY};

		DrawTexturePro(zombieTex, src, dstCorrected, origin,
					   rotation + textureRotation, RAYWHITE);
	}
};

class ZombieManager {
  public:
	void UpdateAll(std::vector<Zombie> &zombies, const LevelMap &map,
				   EffectManager &effectManager, Vector2 playerPos) {
		for (auto &z : zombies) {
			if (z.damageTaken > 0) {
				effectManager.SpawnText(z.position,
										std::to_string(z.damageTaken), RED);
				z.damageTaken = 0;
			}
		}

		zombies.erase(std::remove_if(zombies.begin(), zombies.end(),
									 [](const Zombie &z) { return !z.alive; }),
					  zombies.end());

		for (auto &z : zombies) {
			z.Update(map, playerPos);
		}
	}

	void ResolveZombieCollision(std::vector<Zombie> &zombies) {
		for (size_t i = 0; i < zombies.size(); i++) {
			for (size_t j = i + 1; j < zombies.size(); j++) {
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

					float pushFactor = 0.05f;

					zombies[i].position.x -= nx * overlap * pushFactor;
					zombies[i].position.y -= ny * overlap * pushFactor;

					zombies[j].position.x += nx * overlap * pushFactor;
					zombies[j].position.y += ny * overlap * pushFactor;
				}
			}
		}
	}
};
