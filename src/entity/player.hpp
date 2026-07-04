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

	float maxHp{100};
	float hp{100};

	int money = 0;

	Vector2 velocity{0, 0};

	float knifeCooldownTimer{0.0f};
	const float knifeCooldownDuration{0.5f};
	float knifeVisualTimer{0.0f};
	Vector2 knifeDir{0, 0};
	bool knifeTriggered{false};

	const float knifeRange{75.0f};
	const int knifeDamage{150};

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
	void HandleKnifeActions(Vector2 worldMousePos);
	void HandleWeaponActions(std::vector<Bullet> &bullets);
	void HandleDoorActions(LevelMap &map);
	void HandlePickableActions();
	void HandleBlockadeActions();
};
