#pragma once

#include "../levelMap.hpp"
#include "../weaponManager.hpp"
#include "bullet.hpp"
#include "pickable.hpp"
#include <raylib.h>
#include <vector>

struct Player {
	Vector2 position;
	const int radius{25};
	const float speed{5.0F};

	float hp{100};

	int money = 100000;

	Vector2 velocity{0, 0};

	WeaponManager weaponManager;

	WeaponSpawner *currentSpawner{nullptr};
	Weapon *spawnerWeapon{nullptr};
	Door *currentDoor{nullptr};
	Pickable *currentPickable{nullptr};
	Blockade *currentBlockade{nullptr};

	void Init();

	void Update(std::vector<Bullet> &bullets, Vector2 worldMousePos,
				LevelMap &map);

	void Render() const;

  private:
	void HandleMovement();
	void HandleWeaponActions();
	void HandleDoorActions(LevelMap &map);
	void HandlePickableActions();
	void HandleBlockadeActions();
};
