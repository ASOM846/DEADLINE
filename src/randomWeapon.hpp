#pragma once

#include "weaponManager.hpp"

struct RandomWeaponSpawner {
  public:
	int pos{};

	WeaponType drawnType{};

	bool isDrawn{false};

	std::string message;

	void Update() {}
};
