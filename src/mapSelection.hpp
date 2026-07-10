#pragma once

#include "button.hpp"
#include "levelMap.hpp"
#include <raylib.h>
#include <vector>

class MapSelection {
  public:
	MapSelection() = default;
	~MapSelection() = default;

	void Init();

	void Reset();

	void Update();
	void Render();

	[[nodiscard]] LevelMap GetRandomMap() const { return maps[0]; }

	[[nodiscard]] bool IsSelected() const { return isSelected; }
	[[nodiscard]] bool ShouldReturn() const { return goBack; }

	[[nodiscard]] LevelMap GetSelectedMap() const { return maps[hoverIndex]; }

  private:
	Button goBackBtn;

	bool isSelected{false};
	bool goBack{false};

	int countX{4};

	int width{240};
	int height{320};
	int hoverIndex{-1};

	std::vector<LevelMap> maps;
};
