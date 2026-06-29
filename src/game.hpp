#pragma once

#include "entity/player.hpp"
#include "entity/zombie.hpp"
#include <vector>

class Game {
  public:
	Game() = default;
	~Game() = default;

	void Init();
	void Reset();

	void Update();
	void Render();

  private:
	Player player;

	std::vector<Zombie> zombies;
};
