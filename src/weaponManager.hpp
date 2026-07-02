#pragma once

#include "entity/bullet.hpp"
#include "raylib.h"
#include <cmath>
#include <string>
#include <vector>

enum class WeaponType {
	PISTOL,
	UZI,
	AK47,
	SNIPER,
	SHOTGUN,
	COUNT,
};

struct WeaponSpawner {
	int pos;
	WeaponType type;
};

struct Weapon {
	Weapon() = default;
	~Weapon() = default;

	std::string name;
	WeaponType type;
	int damage{};
	int bullets{};
	int maxAmmo{};
	int ammo{};
	int maxMagazine{};
	int currentMagazine{};
	float reloadTime{};
	float spread{};
	int pierce{};
	float fireRate{};
	bool isAutomatic{};

	int price{};
	int magazinePrice{};
};

struct WeaponManager {
	std::vector<Weapon> inventory;

	int currentWeaponIndex = 0;
	int secondatyWeaponIndex = 3;

	float shootTimer{0.0F};
	float reloadTimer{0.0F};

	bool isReloading{false};

	void Init() { InitInventory(); }

	Weapon *GetCurrentWeapon() {
		if (inventory.empty())
			return nullptr;

		return &inventory[currentWeaponIndex];
	}

	void SwitchWeaponNext() {
		int temp = secondatyWeaponIndex;
		secondatyWeaponIndex = currentWeaponIndex;
		currentWeaponIndex = temp;
	}

	void SwitchWeaponTo(WeaponType newWeapon) {
		for (int i = 0; i < inventory.size(); i++) {
			if (inventory[i].type == newWeapon) {
				isReloading = false;
				reloadTimer = 0;
				currentWeaponIndex = i;
				return;
			}
		}
	}

	Weapon *GetWeapon(WeaponType weaponType) {
		for (auto &i : inventory) {
			if (i.type == weaponType) {
				return &i;
			}
		}
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

	void InitInventory() {
		Weapon pistol;
		pistol.name = "Pistol";
		pistol.type = WeaponType::PISTOL;
		pistol.damage = 20;
		pistol.bullets = 1;
		pistol.maxAmmo = 70;
		pistol.ammo = 10;
		pistol.maxMagazine = 7;
		pistol.currentMagazine = 7;
		pistol.reloadTime = 1.5f;
		pistol.spread = 0.0F;
		pistol.fireRate = 0.5f;
		pistol.pierce = 0;
		pistol.isAutomatic = false;

		pistol.price = 0;
		pistol.magazinePrice = 5;
		inventory.push_back(pistol);

		Weapon uzi;
		uzi.name = "UZI";
		uzi.type = WeaponType::UZI;
		uzi.damage = 20;
		uzi.bullets = 1;
		uzi.maxAmmo = 180;
		uzi.ammo = 90;
		uzi.maxMagazine = 30;
		uzi.currentMagazine = 30;
		uzi.reloadTime = 2.2f;
		uzi.spread = 10.0;
		uzi.fireRate = 0.1f;
		uzi.pierce = 0;
		uzi.isAutomatic = true;

		uzi.price = 400;
		uzi.magazinePrice = 20;
		inventory.push_back(uzi);

		Weapon Ak47;
		Ak47.name = "AK-47";
		Ak47.type = WeaponType::AK47;
		Ak47.damage = 34;
		Ak47.bullets = 1;
		Ak47.maxAmmo = 120;
		Ak47.ammo = 90;
		Ak47.maxMagazine = 30;
		Ak47.currentMagazine = 30;
		Ak47.reloadTime = 3.0f;
		Ak47.spread = 4.0f;
		Ak47.fireRate = 0.15;
		Ak47.pierce = 1;
		Ak47.isAutomatic = true;

		Ak47.price = 1000;
		Ak47.magazinePrice = 40;
		inventory.push_back(Ak47);

		Weapon sniper;
		sniper.name = "Sniper";
		sniper.type = WeaponType::SNIPER;
		sniper.bullets = 1;
		sniper.maxAmmo = 20;
		sniper.ammo = 20;
		sniper.maxMagazine = 5;
		sniper.currentMagazine = 5;
		sniper.reloadTime = 4.0f;
		sniper.damage = 100;
		sniper.spread = 0.0F;
		sniper.fireRate = 1.5f;
		sniper.pierce = 3;
		sniper.isAutomatic = false;

		sniper.price = 1600;
		sniper.magazinePrice = 50;
		inventory.push_back(sniper);

		Weapon shotgun;
		shotgun.name = "Shotgun";
		shotgun.type = WeaponType::SHOTGUN;
		shotgun.bullets = 5;
		shotgun.maxAmmo = 30;
		shotgun.ammo = 25;
		shotgun.maxMagazine = 5;
		shotgun.currentMagazine = 5;
		shotgun.reloadTime = 2.5;
		shotgun.damage = 25;
		shotgun.spread = 25.0f;
		shotgun.fireRate = 1.0f;
		shotgun.pierce = 2;
		shotgun.isAutomatic = false;

		shotgun.price = 700;
		shotgun.magazinePrice = 25;
		inventory.push_back(shotgun);
	}
};
