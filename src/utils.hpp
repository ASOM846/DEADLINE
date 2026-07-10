#include "levelMap.hpp"
#include <raylib.h>

bool CheckLineOfSight(Vector2 start, Vector2 end, const LevelMap &map) {
	float dist =
		std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
	int steps = static_cast<int>(dist / (map.cellSize / 2.0f));

	for (int i = 0; i <= steps; i++) {
		float t = static_cast<float>(i) / steps;
		Vector2 point = {start.x + (end.x - start.x) * t,
						 start.y + (end.y - start.y) * t};

		int tx = static_cast<int>(point.x / map.cellSize);
		int ty = static_cast<int>(point.y / map.cellSize);

		if (tx < 0 || tx >= map.width || ty < 0 || ty >= map.height)
			return false;

		TileType tile = map.tiles[ty * map.width + tx];

		if (tile == TileType::WALL || LevelMap::IsDoor(tile))
			return false;
	}
	return true;
}
