#pragma once

#include "entity/bullet.hpp"
#include "raylib.h"
#include <cmath>
#include <string>
#include <vector>

struct Weapon {
	Weapon() = default;
	~Weapon() = default;

	std::string name;
	int damage{};
	int bullets{};
	int ammo{};
	int maxMagazine{};
	int currentMagazine{};
	float reloadTime{};
	float spread{};
	int pierce{};
	float fireRate{};
	bool isAutomatic{};
};

struct WeaponManager {
	std::vector<Weapon> inventory;
	int currentWeaponIndex = 0;

	float shootTimer{0.0F};
	float reloadTimer{0.0F};

	bool isReloading{false};

	Weapon *GetCurrentWeapon() {
		if (inventory.empty())
			return nullptr;

		return &inventory[currentWeaponIndex];
	}

	void SwitchWeaponNext() {
		if (inventory.empty())
			return;

		isReloading = false;
		reloadTimer = 0.0F;

		currentWeaponIndex = (currentWeaponIndex + 1) % inventory.size();
	}

	void StartReload() {
		Weapon *w = GetCurrentWeapon();
		if (w && w->ammo > 0 && w->currentMagazine < w->maxMagazine &&
			!isReloading) {
			isReloading = true;
			reloadTimer = 0.0F;
		}
	}

	void Update(std::vector<Bullet> &bullets, Vector2 startPos,
				Vector2 targetPos) {
		Weapon *w = GetCurrentWeapon();
		if (w == nullptr) {
			return;
		}

		shootTimer += GetFrameTime();

		if (isReloading) {
			reloadTimer += GetFrameTime();
			if (reloadTimer >= w->reloadTime) {
				isReloading = false;
				int remainingRounds = w->maxMagazine - w->currentMagazine;

				if (w->ammo < remainingRounds) {
					w->currentMagazine += w->ammo;
					w->ammo = 0;
				} else {
					w->currentMagazine += remainingRounds;
					w->ammo -= remainingRounds;
				}
				reloadTimer = 0.0f;
			}
			return;
		}

		if (w->currentMagazine == 0 && w->ammo > 0) {
			StartReload();
			return;
		}

		if (shootTimer < w->fireRate)
			return;

		bool wantsToShoot = w->isAutomatic
								? IsMouseButtonDown(MOUSE_BUTTON_LEFT)
								: IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

		if (wantsToShoot && w->currentMagazine > 0) {
			FireWeapon(bullets, startPos, targetPos, w);
			shootTimer = 0.0F;
			w->currentMagazine--;
		}
	}

  private:
	void FireWeapon(std::vector<Bullet> &bullets, Vector2 startPos,
					Vector2 targetPos, Weapon *w) const {
		for (int i = 0; i < w->bullets; i++) {
			float angle =
				atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);

			float spread = w->spread;

			float spreadOffset = GetRandomValue(-100, 100) / 100.0F * spread;

			angle += spreadOffset * DEG2RAD;

			float dirX = cosf(angle);
			float dirY = sinf(angle);

			bullets.emplace_back(startPos, dirX, dirY, w->damage, w->pierce);
		}
	}
};
