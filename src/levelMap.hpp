#pragma once

#include "baricade.hpp"
#include "potion.hpp"
#include "randomWeapon.hpp"
#include "textureManager.hpp"
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

struct Door {
	int pos{};
	int price{};
	bool open{false};
};

enum class TileType {
	FLOOR,
	WALL,
	ZOMBIE_SPAWNER,
	BLOCKADE,

	PLAYER_SPAWN,

	DOOR_200,
	DOOR_500,
	DOOR_750,
	DOOR_1000,
	DOOR_1250,
	DOOR_1500,
	DOOR_1750,
	DOOR_2000,

	POTION_SPEED,
	POTION_THIRD_SLOT,
	POTION_RAPID_FIRE,
	POTION_HEAL,

	WEAPON_UZI,
	WEAPON_AK47,
	WEAPON_SNIPER,
	WEAPON_SHOTGUN,

	WEAPON_RANDOM,

	COUNT
};

struct LevelMap {
	const int width = 100;
	const int height = 100;
	const int cellSize = 40;
	const std::string filename = "assets/map.txt";
	const int INF = 9999;

	std::vector<TileType> tiles;
	std::vector<int> distanceMap;
	std::vector<int> bitmaskMap;

	Vector2 playerSpawnPos;

	std::vector<int> zombieSpawners;
	std::vector<WeaponSpawner> weaponSpawners;
	std::vector<RandomWeaponSpawner> randomWeaponSpawners;
	std::vector<Door> doors;
	std::vector<Blockade> blockades;
	std::vector<PotionSpawner> potionSpawns;

  public:
	static bool IsDoor(TileType type) {
		return (type >= TileType::DOOR_200 && type <= TileType::DOOR_2000);
	}

	static bool IsPotion(TileType type) {
		return (type >= TileType::POTION_SPEED &&
				type <= TileType::POTION_HEAL);
	}

	void Init() {
		tiles.resize(width * height, TileType::FLOOR);
		distanceMap.resize(width * height, INF);
		bitmaskMap.resize(width * height, -1);

		LoadMap();
		CalculateBitmask();
	}

	void LoadMap() {
		std::ifstream inputFile("assets/map.txt");

		if (!inputFile.is_open()) {
			TraceLog(LOG_ERROR, "UBALBE TO LOAD FILE");
		}

		int tileValue;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (inputFile >> tileValue) {
					tiles[y * width + x] = static_cast<TileType>(tileValue);
				}
			}
		}

		inputFile.close();

		zombieSpawners.clear();
		doors.clear();
		weaponSpawners.clear();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				auto &tile = tiles[y * width + x];
				int index = y * width + x;

				if (tile == TileType::PLAYER_SPAWN) {
					playerSpawnPos = {.x = static_cast<float>(x * cellSize),
									  .y = static_cast<float>(y * cellSize)};
				}

				if (tile == TileType::ZOMBIE_SPAWNER) {
					zombieSpawners.push_back(y * width + x);
				}

				if (IsDoor(tile)) {
					doors.push_back(Door{.pos = index,
										 .price = GetDoorPrice(tile),
										 .open = false});
				}

				if (IsPotion(tile)) {
					potionSpawns.push_back(PotionSpawner{
						.pos = index, .type = GetPotionType(tile)});
				}

				if (tile == TileType::BLOCKADE) {
					blockades.push_back({.pos = y * width + x, .hp = 100});
				}

				if (tile == TileType::WEAPON_UZI) {
					weaponSpawners.push_back(
						{.pos = (y * width) + x, .type = WeaponType::UZI});
				}

				if (tile == TileType::WEAPON_AK47) {
					weaponSpawners.push_back(
						{.pos = (y * width) + x, .type = WeaponType::AK47});
				}

				if (tile == TileType::WEAPON_SNIPER) {
					weaponSpawners.push_back(
						{.pos = (y * width) + x, .type = WeaponType::SNIPER});
				}

				if (tile == TileType::WEAPON_SHOTGUN) {
					weaponSpawners.push_back(
						{.pos = (y * width) + x, .type = WeaponType::SHOTGUN});
				}

				if (tile == TileType::WEAPON_RANDOM) {
					randomWeaponSpawners.push_back(
						RandomWeaponSpawner{.pos = (y * width) + x});
				}
			}
		}
	}

	void Update(Vector2 playerPos) { UpdateFloodField(playerPos); }

	void Render(TextureManager &tm) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				TileType type = tiles[y * width + x];

				if (type == TileType::FLOOR) {
					TextureId id = TextureId::FLOOR;

					Texture2D floorTex = tm.get(id);

					Rectangle dst = {static_cast<float>(x * cellSize),
									 static_cast<float>(y * cellSize),
									 static_cast<float>(cellSize),
									 static_cast<float>(cellSize)};

					Rectangle src = {0, 0, static_cast<float>(floorTex.width),
									 static_cast<float>(floorTex.height)};

					DrawTexturePro(floorTex, src, dst, {0, 0}, 0.0f, RAYWHITE);
				}

				if (type != TileType::FLOOR) {
					if (type != TileType::WALL) {
						DrawRectangle(x * cellSize, y * cellSize, cellSize,
									  cellSize, GetTileColor(type));
					} else if (type == TileType::WALL) {
						TextureId id = GetWallTextureId(x, y);

						Texture2D walltex = tm.get(id);

						Rectangle dst = {static_cast<float>(x * cellSize),
										 static_cast<float>(y * cellSize),
										 static_cast<float>(cellSize),
										 static_cast<float>(cellSize)};

						Rectangle src = {0, 0,
										 static_cast<float>(walltex.width),
										 static_cast<float>(walltex.height)};

						DrawTexturePro(walltex, src, dst, {0, 0}, 0.0f,
									   RAYWHITE);
					}
				}
			}
		}
	}

  private:
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

  private:
	static int GetDoorPrice(TileType type) {
		switch (type) {
		case TileType::DOOR_200:
			return 200;
		case TileType::DOOR_500:
			return 500;
		case TileType::DOOR_750:
			return 750;
		case TileType::DOOR_1000:
			return 1000;
		case TileType::DOOR_1250:
			return 1250;
		case TileType::DOOR_1500:
			return 1500;
		case TileType::DOOR_1750:
			return 1750;
		case TileType::DOOR_2000:
			return 2000;
			break;
		}
	}

	static PotionType GetPotionType(TileType type) {
		switch (type) {
		case TileType::POTION_SPEED:
			return PotionType::SPEED;
		case TileType::POTION_THIRD_SLOT:
			return PotionType::THIRD_SLOT;
		case TileType::POTION_RAPID_FIRE:
			return PotionType::RAPID_FIRE;
		case TileType::POTION_HEAL:
			return PotionType::HEAL;
		}
	}

	static Color GetTileColor(TileType type) {
		switch (type) {
		case TileType::FLOOR:
			return RAYWHITE;
		case TileType::WALL:
			return RED;
		case TileType::ZOMBIE_SPAWNER:
			return GREEN;
		case TileType::BLOCKADE:
			return YELLOW;
		case TileType::PLAYER_SPAWN:
			return GRAY;
		case TileType::DOOR_200:
			return DARKGREEN;
		case TileType::DOOR_500:
			return ORANGE;
		case TileType::DOOR_750:
			return GOLD;
		case TileType::DOOR_1000:
			return ColorAlpha(ORANGE, 0.5);
		case TileType::DOOR_1250:
			return BROWN;
		case TileType::DOOR_1500:
			return DARKBROWN;
		case TileType::DOOR_1750:
			return BEIGE;
		case TileType::DOOR_2000:
			return PURPLE;
		case TileType::POTION_SPEED:
			return SKYBLUE;
		case TileType::POTION_THIRD_SLOT:
			return MAGENTA;
		case TileType::POTION_RAPID_FIRE:
			return ORANGE;
		case TileType::POTION_HEAL:
			return LIME;
		case TileType::WEAPON_UZI:
			return MAGENTA;
		case TileType::WEAPON_AK47:
			return MAROON;
		case TileType::WEAPON_SNIPER:
			return PINK;
		case TileType::WEAPON_SHOTGUN:
			return SKYBLUE;
		case TileType::WEAPON_RANDOM:
			return BLACK;
		case TileType::COUNT:
			break;
		default:
			return BLACK;
		}
		return BLACK;
	}

	bool IsWall(int checkX, int checkY) {
		if (checkX < 0 || checkX >= width || checkY < 0 || checkY >= height)
			return false;

		return tiles[checkY * width + checkX] == TileType::WALL;
	}

	void CalculateBitmask() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int idx = y * width + x;

				if (IsWall(x, y)) {
					int bitmask = 0;

					if (IsWall(x, y - 1))
						bitmask += 1;
					if (IsWall(x + 1, y))
						bitmask += 2;
					if (IsWall(x, y + 1))
						bitmask += 4;
					if (IsWall(x - 1, y))
						bitmask += 8;

					bitmaskMap[idx] = bitmask;
				}
			}
		}
	}

	TextureId GetWallTextureId(int x, int y) {
		int bitmask = bitmaskMap[y * width + x];

		TextureId resoult;

		//    |   | 1 |   |
		//    | 8 |pos| 2 |
		//    |   | 4 |   |

		switch (bitmask) {
		case 0:
			resoult = TextureId::WALL_SINGLE;
			break;
		case 1:
			resoult = TextureId::WALL_END_TOP;
			break;
		case 2:
			resoult = TextureId::WALL_END_RIGHT;
			break;
		case 3:
			resoult = TextureId::WALL_JOIN_TOP_RIGHT;
			break;
		case 4:
			resoult = TextureId::WALL_END_BOTTOM;
			break;
		case 5:
			resoult = TextureId::WALL_JOIN_BOTTOM_TOP;
			break;
		case 6:
			resoult = TextureId::WALL_JOIN_BOTTOM_RIGHT;
			break;
		case 7:
			resoult = TextureId::WALL_JOIN_BOTTOM_RIGHT_TOP;
			break;
		case 8:
			resoult = TextureId::WALL_END_LEFT;
			break;
		case 9:
			resoult = TextureId::WALL_JOIN_TOP_LEFT;
			break;
		case 10:
			resoult = TextureId::WALL_STRAIGHT;
			break;
		case 11:
			resoult = TextureId::WALL_JOIN_LEFT_TOP_RIGHT;
			break;
		case 12:
			resoult = TextureId::WALL_JOIN_BOTTOM_LEFT;
			break;
		case 13:
			resoult = TextureId::WALL_JOIN_BOTTOM_LEFT_TOP;
			break;
		case 14:
			resoult = TextureId::WALL_JOIN_BOTTOM_LEFT_RIGHT;
			break;
		case 15:
			resoult = TextureId::WALL_ALL;
			break;
		}

		return resoult;
	}
};
