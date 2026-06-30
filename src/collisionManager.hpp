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
							 std::vector<Zombie> &zombies) {
		for (auto zombie = zombies.begin(); zombie != zombies.end();) {
			for (auto bullet = bullets.begin(); bullet != bullets.end();) {
				if (CheckCollisionCircles(zombie->position, zombie->width / 2,
										  bullet->position, bullet->radius)) {
					zombie->position.x += bullet->dirX * bullet->knockbackForce;
					zombie->position.y += bullet->dirY * bullet->knockbackForce;

					zombie->hp -= bullet->damage;

					bullet = bullets.erase(bullet);
				} else {
					bullet++;
				}
			}
			zombie++;
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
};
