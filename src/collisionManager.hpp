#pragma once

#include "entity/bullet.hpp"
#include "entity/player.hpp"
#include "entity/zombie.hpp"
#include "levelMap.hpp"
#include "raylib.h"
#include <algorithm>
#include <vector>

class CollisionManager {
  public:
	CollisionManager() = default;
	~CollisionManager() = default;

	void ResolveBulletZombie(std::vector<Bullet> &bullets,
							 std::vector<Zombie> &zombies, Player &player) {
		for (auto &z : zombies) {
			for (auto &b : bullets) {
				if (!b.alive)
					continue;
				if (!z.alive)
					continue;

				if (CheckCollisionCircles(z.position, z.radius, b.position,
										  b.radius)) {

					z.position.x += b.dirX * b.knockbackForce;
					z.position.y += b.dirY * b.knockbackForce;

					z.hp -= b.damage;

					b.pierce--;

					if (z.hp <= 0) {
						player.money += z.value;
					}

					if (b.pierce < 0) {
						b.alive = false;
					}
				}
			}
		}
	}

	void ResolvePlayerWall(Player &player, LevelMap &map) {
		player.position.x += player.velocity.x;
		player.position.y += player.velocity.y;

		for (int y = 0; y < map.height; y++) {
			for (int x = 0; x < map.width; x++) {

				if (map.tiles[y * map.width + x] == TileType::WALL ||
					map.tiles[y * map.width + x] == TileType::BLOCKADE) {

					Rectangle wallRec = {static_cast<float>(x * map.cellSize),
										 static_cast<float>(y * map.cellSize),
										 static_cast<float>(map.cellSize),
										 static_cast<float>(map.cellSize)};

					if (CheckCollisionCircleRec(player.position, player.radius,
												wallRec)) {

						float closestX =
							std::clamp(player.position.x, wallRec.x,
									   wallRec.x + wallRec.width);
						float closestY =
							std::clamp(player.position.y, wallRec.y,
									   wallRec.y + wallRec.height);

						float dirX = player.position.x - closestX;
						float dirY = player.position.y - closestY;

						float distance = std::sqrt(dirX * dirX + dirY * dirY);

						if (distance < player.radius) {

							if (distance == 0.0f) {
								dirX = 0.0f;
								dirY = -1.0f;
								distance = 1.0f;
							}

							dirX /= distance;
							dirY /= distance;

							float overlap = player.radius - distance;

							player.position.x += dirX * overlap;
							player.position.y += dirY * overlap;
						}
					}
				}
			}
		}
	}

	void ResolvePlayerWeaponSpawner(Player &player, LevelMap &map) {
		player.currentSpawner = nullptr;
		player.spawnerWeapon = nullptr;

		for (auto &spawner : map.weaponSpawners) {
			Rectangle spawnerRec = {
				static_cast<float>(spawner.pos % map.width * map.cellSize),
				static_cast<float>(spawner.pos / map.width * map.cellSize),
				static_cast<float>(map.cellSize),
				static_cast<float>(map.cellSize)};

			if (CheckCollisionCircleRec(player.position, player.radius,
										spawnerRec)) {
				player.currentSpawner = &spawner;
				player.spawnerWeapon =
					player.weaponManager.GetWeapon(spawner.type);
				break;
			}
		}
	}

	void ResolveBulletWall(std::vector<Bullet> &bullets, LevelMap &map) {
		int index = 0;
		for (auto &w : map.tiles) {
			if (w != TileType::WALL) {
				index++;
				continue;
			}

			int gridX = (index % map.width) * map.cellSize;
			int gridY = (index / map.width) * map.cellSize;

			Rectangle wallRec = {static_cast<float>(gridX),
								 static_cast<float>(gridY),
								 static_cast<float>(map.cellSize),
								 static_cast<float>(map.cellSize)};

			for (auto bullet = bullets.begin(); bullet != bullets.end();) {
				if (CheckCollisionCircleRec(bullet->position, bullet->radius,
											wallRec)) {
					bullet = bullets.erase(bullet);
				} else {
					bullet++;
				}
			}
			index++;
		}
	}

	void ResolvePlayerZombie(Player &player, std::vector<Zombie> &zombies) {
		for (auto &z : zombies) {
			if (CheckCollisionCircles(player.position, player.radius,
									  z.position, z.radius)) {
				player.hp -= z.damage * GetFrameTime();
				TraceLog(LOG_INFO,
						 TextFormat("COLLISION DETECTED    %f", player.hp));
			}
		}
	}
};
