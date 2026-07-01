#pragma once

#include "weaponManager.hpp"
#include <fstream>
#include <queue>
#include <raylib.h>
#include <string>
#include <vector>

struct Point {
	int x;
	int y;
};

enum class TileType {
	FLOOR,
	WALL,
	ZOMBIE_SPAWNER,
	BLOCKADE,
	WEAPON_SHOP
};

struct LevelMap {
	const int width = 90;
	const int height = 50;
	const int cellSize = 40;
	const std::string filename = "assets/map.txt";
	const int INF = 9999;

	std::vector<TileType> tiles;
	std::vector<int> distanceMap;

	std::vector<int> zombieSpawners;
	std::vector<WeaponSpawner> weaponSpawners;

	void Init() {
		tiles.resize(width * height, TileType::FLOOR);
		distanceMap.resize(width * height, INF);

		LoadMap();
	}

	void LoadMap() {
		std::ifstream inputFile("assets/map.txt");

		if (!inputFile.is_open()) {
			TraceLog(LOG_ERROR, "UBALBE TO LOAD FILE");
		}

		char c;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				inputFile >> c;
				tiles[y * width + x] = static_cast<TileType>(c - '0');
			}
		}

		inputFile.close();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (tiles[y * width + x] == TileType::ZOMBIE_SPAWNER) {
					zombieSpawners.push_back(y * width + x);
				}

				if (tiles[y * width + x] == TileType::WEAPON_SHOP) {
					int seed =
						GetRandomValue(0, static_cast<int>(WeaponType::COUNT));
					weaponSpawners.push_back(
						{.pos = (y * width) + x,
						 .type = static_cast<WeaponType>(seed)});
				}
			}
		}
	}

	void Update(Vector2 playerPos) { UpdateFloodField(playerPos); }

	void Render() const {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				switch (tiles[y * width + x]) {
				case TileType::FLOOR:
					break;
				case TileType::WALL:
					DrawRectangle(x * cellSize, y * cellSize, cellSize,
								  cellSize, YELLOW);
					break;
				case TileType::WEAPON_SHOP:
					DrawRectangle(x * cellSize, y * cellSize, cellSize,
								  cellSize, GREEN);
					break;
				}

				int index = y * width + x;
				int distance = distanceMap[index];
			}
		}
	}

	void UpdateFloodField(Vector2 playerPos) {
		distanceMap.assign(width * height, INF);

		int pGridX = static_cast<int>(playerPos.x / cellSize);
		int pGridY = static_cast<int>(playerPos.y / cellSize);

		if (pGridX < 0 || pGridX >= width || pGridY < 0 || pGridY >= height) {
			return;
		}

		std::queue<Point> queue;

		int playerIndex = pGridY * width + pGridX;
		distanceMap[playerIndex] = 0;
		queue.push({pGridX, pGridY});

		int dx[] = {0, 0, -1, 1};
		int dy[] = {-1, 1, 0, 0};

		while (!queue.empty()) {
			Point current = queue.front();
			queue.pop();

			int currentIndex = current.y * width + current.x;
			int currentDist = distanceMap[currentIndex];

			for (int i = 0; i < 4; i++) {
				int nx = current.x + dx[i];
				int ny = current.y + dy[i];

				if (nx >= 0 && nx < width && ny >= 0 && ny < height) {

					int nIndex = ny * width + nx;

					if ((tiles[nIndex] == TileType::FLOOR ||
						 tiles[nIndex] == TileType::BLOCKADE) &&
						distanceMap[nIndex] == INF) {
						distanceMap[nIndex] = currentDist + 1;
						queue.push({nx, ny});
					}
				}
			}
		}
	}
};
