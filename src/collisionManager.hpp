#pragma once

#include "entity/bullet.hpp"
#include "entity/player.hpp"
#include "entity/zombie.hpp"
#include "levelMap.hpp"
#include "raylib.h"
#include <algorithm>
#include <filesystem>
#include <numbers>
#include <stdexcept>
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
					map.tiles[y * map.width + x] == TileType::BLOCKADE ||
					map.IsDoor(map.tiles[y * map.width + x])) {

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

	void ResolvePlayerBlockade(Player &player, LevelMap &map) {
		int interactionOffset = 10;

		player.currentBlockade = nullptr;

		for (auto &b : map.blockades) {
			Rectangle rec = {
				static_cast<float>((b.pos % map.width) * map.cellSize -
								   interactionOffset),
				static_cast<float>((b.pos / map.width) * map.cellSize -
								   interactionOffset),
				static_cast<float>(map.cellSize + interactionOffset * 2),
				static_cast<float>(map.cellSize + interactionOffset * 2)};

			if (CheckCollisionCircleRec(player.position, player.radius, rec)) {
				player.currentBlockade = &b;
				return;
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

	void ResolvePlayerDoor(Player &player, LevelMap &map) {
		player.currentDoor = nullptr;

		int interactionOffset = 10;

		for (auto &door : map.doors) {
			Rectangle doorRec = {
				static_cast<float>((door.pos % map.width) * map.cellSize -
								   interactionOffset),
				static_cast<float>((door.pos / map.width) * map.cellSize -
								   interactionOffset),
				static_cast<float>(map.cellSize + interactionOffset * 2),
				static_cast<float>(map.cellSize + interactionOffset * 2)};

			if (CheckCollisionCircleRec(player.position, player.radius,
										doorRec)) {
				player.currentDoor = &door;
				break;
			}
		}
	}

	void ResolvePlayerPickable(Player &player,
							   std::vector<Pickable> &pickables) {
		player.currentPickable = nullptr;

		for (auto &pickable : pickables) {
			if (CheckCollisionCircles(player.position, player.radius,
									  pickable.position,
									  pickable.currentRadius)) {
				player.currentPickable = &pickable;
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
			}
		}
	}

	void ResolveZombieBlockade(std::vector<Zombie> &zombies, LevelMap &map) {
		for (auto &blockade : map.blockades) {
			if (blockade.hp <= 0.0F) {
				continue;
			}

			int gridX = (blockade.pos % map.width) * map.cellSize;
			int gridY = (blockade.pos / map.width) * map.cellSize;

			Rectangle blockRec = {static_cast<float>(gridX),
								  static_cast<float>(gridY),
								  static_cast<float>(map.cellSize),
								  static_cast<float>(map.cellSize)};

			for (auto &z : zombies) {
				if (!z.alive)
					continue;

				if (CheckCollisionCircleRec(z.position, z.radius, blockRec)) {
					blockade.hp -= z.damage * GetFrameTime();

					float cloasestX = std::clamp(z.position.x, blockRec.x,
												 blockRec.x + blockRec.width);
					float cloasestY = std::clamp(z.position.y, blockRec.y,
												 blockRec.y + blockRec.height);

					float distX = z.position.x - cloasestX;
					float distY = z.position.y - cloasestY;
					float distance = std::sqrt(distX * distX + distY * distY);

					if (distance < z.radius) {
						if (distance == 0.0f) {
							z.position.y -= z.radius;
						} else {
							float overlap = z.radius - distance;
							z.position.x += (distX / distance) * overlap;
							z.position.y += (distY / distance) * overlap;
						}
					}
				}
			}
		}
	}
};
