#pragma once

#include <vector>
struct LevelMap {
	const int width = 20;
	const int height = 15;
	std::vector<int> tiles;
	std::vector<int> distanceMap;
};
