#pragma once

#include "levelMap.hpp"
#include <raylib.h>
#include <vector>

class MapSelection {
  public:
	MapSelection() = default;
	~MapSelection() = default;

	void Init();

	void Update();
	void Render();

	[[nodiscard]] bool IsSelected() const { return isSelected; }

	[[nodiscard]] LevelMap GetSelectedMap() const { return maps[hoverIndex]; }

  private:
	bool isSelected{false};

	int countX{4};

	int width{240};
	int height{320};
	int hoverIndex{-1};

	std::vector<LevelMap> maps;
};
