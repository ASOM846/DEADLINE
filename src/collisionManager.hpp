#pragma once

#include "entity/bullet.hpp"
#include "entity/pickable.hpp"
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
							 std::vector<Zombie> &zombies, Player &player,
							 std::vector<Pickable> &pickables,
							 PickableManager &pickableManager) {
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
					z.damageTaken += b.damage;

					b.pierce--;

					if (z.hp <= 0) {
						player.money += z.value;
						pickableManager.TrySpawnDrop(pickables, z.position,
													 z.dropChance);
					}

					if (b.pierce < 0) {
						b.alive = false;
					}
				}
			}
		}
	}
	void ResolveEntityWall(Vector2 &position, float radius, LevelMap &map,
						   bool isZombie) {
		int gridX = static_cast<int>(position.x) / map.cellSize;
		int gridY = static_cast<int>(position.y) / map.cellSize;

		int startX = std::max(0, gridX - 1);
		int endX = std::min(map.width - 1, gridX + 1);
		int startY = std::max(0, gridY - 1);
		int endY = std::min(map.height - 1, gridY + 1);

		for (int y = startY; y <= endY; y++) {
			for (int x = startX; x <= endX; x++) {
				TileType currentTile = map.tiles[y * map.width + x];

				if (currentTile == TileType::WALL ||
					(!isZombie && currentTile == TileType::BLOCKADE) ||
					LevelMap::IsDoor(currentTile)) {

					Rectangle wallRec = {static_cast<float>(x * map.cellSize),
										 static_cast<float>(y * map.cellSize),
										 static_cast<float>(map.cellSize),
										 static_cast<float>(map.cellSize)};

					if (CheckCollisionCircleRec(position, radius, wallRec)) {
						float closestX = std::clamp(position.x, wallRec.x,
													wallRec.x + wallRec.width);

						float closestY = std::clamp(position.y, wallRec.y,
													wallRec.y + wallRec.height);

						float dirX = position.x - closestX;
						float dirY = position.y - closestY;
						float distance = std::sqrt(dirX * dirX + dirY * dirY);

						if (distance < radius) {
							if (distance == 0.0f) {
								dirX = 0.0f;
								dirY = -1.0f;
								distance = 1.0f;
							}

							dirX /= distance;
							dirY /= distance;
							float overlap = radius - distance;

							position.x += dirX * overlap;
							position.y += dirY * overlap;
						}
					}
				}
			}
		}
	}

	void ResolvePlayerWall(Player &player, LevelMap &map) {
		player.position.x += player.velocity.x;
		player.position.y += player.velocity.y;

		ResolveEntityWall(player.position, player.radius, map, false);
	}

	void ResolveZombieWall(std::vector<Zombie> &zombies, LevelMap &map) {
		for (auto &z : zombies) {
			if (!z.alive)
				continue;
			ResolveEntityWall(z.position, z.radius, map, true);
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
					player.weaponManager.GetWeaponTemplate(spawner.type);

				break;
			}
		}
	}

	void ResolvePlayerRandomWeaponSpawner(Player &player, LevelMap &map) {
		player.currentRandomSpawner = nullptr;

		for (auto &spawner : map.randomWeaponSpawners) {
			Rectangle spawnerRec = {
				static_cast<float>(spawner.pos % map.width * map.cellSize),
				static_cast<float>(spawner.pos / map.width * map.cellSize),
				static_cast<float>(map.cellSize),
				static_cast<float>(map.cellSize)};

			if (CheckCollisionCircleRec(player.position, player.radius,
										spawnerRec)) {
				player.currentRandomSpawner = &spawner;
			}
		}
	}

	void ResolvePlayerPotionSpawner(Player &player, LevelMap &map) {
		player.currentPotionSpawn = nullptr;

		for (auto &spawner : map.potionSpawns) {
			Rectangle rect = {
				static_cast<float>(spawner.pos % map.width * map.cellSize),
				static_cast<float>(spawner.pos / map.width * map.cellSize),
				static_cast<float>(map.cellSize),
				static_cast<float>(map.cellSize)};

			if (CheckCollisionCircleRec(player.position, player.radius, rect)) {
				player.currentPotionSpawn = &spawner;
				return;
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
				pickable.alive = false;
				break;
			}
		}
	}

	void ResolvePlayerKnife(Player &player, std::vector<Zombie> &zombies,
							PickableManager &pickableManager,
							std::vector<Pickable> &pickables) {
		if (!player.knifeTriggered)
			return;

		player.knifeTriggered = false;

		for (auto &z : zombies) {
			if (!z.alive)
				continue;

			float dx = z.position.x - player.position.x;
			float dy = z.position.y - player.position.y;
			float dist = std::sqrt(dx * dx + dy * dy);

			if (dist <= player.knifeRange + z.radius) {
				if (dist > 0.0f) {
					float nx = dx / dist;
					float ny = dy / dist;

					float dotProduct =
						nx * player.knifeDir.x + ny * player.knifeDir.y;

					if (dotProduct > 0.5f) {
						z.hp -= player.knifeDamage;
						z.damageTaken += player.knifeDamage;

						z.position.x += player.knifeDir.x * 50.0f;
						z.position.y += player.knifeDir.y * 50.0f;

						if (z.hp <= 0) {
							z.alive = false;
							player.money += 20;
							pickableManager.TrySpawnDrop(pickables, z.position,
														 z.dropChance);
						}
					}
				}
			}
		}
	}

	void ResolveBulletWall(std::vector<Bullet> &bullets, LevelMap &map) {
		for (auto bullet = bullets.begin(); bullet != bullets.end();) {
			int gridX = static_cast<int>(bullet->position.x) / map.cellSize;
			int gridY = static_cast<int>(bullet->position.y) / map.cellSize;

			if (gridX >= 0 && gridX < map.width && gridY >= 0 &&
				gridY < map.height) {
				int tileIndex = gridY * map.width + gridX;
				TileType tile = map.tiles[tileIndex];

				if (tile == TileType::WALL || LevelMap::IsDoor(tile)) {
					bullet = bullets.erase(bullet);
					continue;
				}
			}
			bullet++;
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
