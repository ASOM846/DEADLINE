#pragma once

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
	BLOCKADE
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
				}

				DrawRectangleLines(x * cellSize, y * cellSize, cellSize,
								   cellSize, MAGENTA);

				int index = y * width + x;
				int distance = distanceMap[index];

				if (distance != INF) {
					std::string distStr = std::to_string(distance);
					int textX = x * cellSize + 12;
					int textY = y * cellSize + 12;

					DrawText(distStr.c_str(), textX, textY, 16, WHITE);
				}
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
