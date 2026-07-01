#pragma once

#include "bullet.hpp"
#include <cmath>
#include <raylib.h>
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

struct Player {
	Vector2 position;
	const int radius{25};
	const float speed{5.0F};

	float shootTimer{0.0F};
	float reloadTimer{0.0F};

	float hp{100};

	bool isReloading{false};

	Vector2 velocity{0, 0};

	std::vector<Weapon> weapons;

	int currentWeaponIndex = 0;

	void Init() {
		Weapon pistol;
		pistol.name = "Pistol";
		pistol.damage = 20;
		pistol.bullets = 1;
		pistol.ammo = 10;
		pistol.maxMagazine = 7;
		pistol.currentMagazine = 7;
		pistol.reloadTime = 1.5f;
		pistol.spread = 0.0F;
		pistol.fireRate = 0.5f;
		pistol.pierce = 0;
		pistol.isAutomatic = false;
		weapons.push_back(pistol);

		Weapon uzi;
		uzi.name = "UZI";
		uzi.damage = 20;
		uzi.bullets = 1;
		uzi.ammo = 90;
		uzi.maxMagazine = 30;
		uzi.currentMagazine = 30;
		uzi.reloadTime = 2.2f;
		uzi.spread = 10.0;
		uzi.fireRate = 0.1f;
		uzi.pierce = 0;
		uzi.isAutomatic = true;
		weapons.push_back(uzi);

		Weapon Ak47;
		Ak47.name = "AK-47";
		Ak47.damage = 34;
		Ak47.bullets = 1;
		Ak47.ammo = 90;
		Ak47.maxMagazine = 30;
		Ak47.currentMagazine = 30;
		Ak47.reloadTime = 3.0f;
		Ak47.spread = 4.0f;
		Ak47.fireRate = 0.15;
		Ak47.pierce = 1;
		Ak47.isAutomatic = true;
		weapons.push_back(Ak47);

		Weapon sniper;
		sniper.name = "Sniper";
		sniper.bullets = 1;
		sniper.ammo = 20;
		sniper.maxMagazine = 5;
		sniper.currentMagazine = 5;
		sniper.reloadTime = 4.0f;
		sniper.damage = 100;
		sniper.spread = 0.0F;
		sniper.fireRate = 1.5f;
		sniper.pierce = 3;
		sniper.isAutomatic = false;
		weapons.push_back(sniper);

		Weapon shotgun;
		shotgun.name = "Shotgun";
		shotgun.bullets = 5;
		shotgun.ammo = 25;
		shotgun.maxMagazine = 5;
		shotgun.currentMagazine = 5;
		shotgun.reloadTime = 2.5;
		shotgun.damage = 25;
		shotgun.spread = 25.0f;
		shotgun.fireRate = 1.0f;
		shotgun.pierce = 2;
		shotgun.isAutomatic = false;
		weapons.push_back(shotgun);

		currentWeaponIndex = 0;
	}

	void Update(std::vector<Bullet> &bullets, Vector2 worldMousePos) {
		velocity = {.x = 0, .y = 0};

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			SwitchWeapon(currentWeaponIndex + 1);
			if (currentWeaponIndex > weapons.size() - 1) {
				SwitchWeapon(0);
			}
		}

		if (IsKeyDown(KEY_R)) {
			isReloading = true;
		}

		if (IsKeyDown(KEY_W)) {
			velocity.y -= speed;
		}

		if (IsKeyDown(KEY_S)) {
			velocity.y += speed;
		}

		if (IsKeyDown(KEY_A)) {
			velocity.x -= speed;
		}

		if (IsKeyDown(KEY_D)) {
			velocity.x += speed;
		}

		if (currentWeaponIndex >= 0) {
			shootTimer += GetFrameTime();

			if (isReloading) {
				reloadTimer += GetFrameTime();
			}

			if (weapons[currentWeaponIndex].ammo == 0) {
				isReloading = false;
			}

			if (reloadTimer >= weapons[currentWeaponIndex].reloadTime) {
				isReloading = false;
				int remainingRounds =
					weapons[currentWeaponIndex].maxMagazine -
					weapons[currentWeaponIndex].currentMagazine;

				if (weapons[currentWeaponIndex].ammo < remainingRounds) {
					weapons[currentWeaponIndex].currentMagazine +=
						weapons[currentWeaponIndex].ammo;
					weapons[currentWeaponIndex].ammo = 0;
				} else {
					weapons[currentWeaponIndex].currentMagazine +=
						remainingRounds;
					weapons[currentWeaponIndex].ammo -= remainingRounds;
				}
				reloadTimer = 0.0F;
			}

			if (shootTimer < weapons[currentWeaponIndex].fireRate) {
				return;
			}

			if (weapons[currentWeaponIndex].isAutomatic && !isReloading &&
				weapons[currentWeaponIndex].currentMagazine > 0) {
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
					FireWeapon(bullets, position, worldMousePos);
					shootTimer = 0.0F;
					weapons[currentWeaponIndex].currentMagazine--;
				}
			} else {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isReloading &&
					weapons[currentWeaponIndex].currentMagazine > 0) {
					FireWeapon(bullets, position, worldMousePos);
					shootTimer = 0.0F;
					weapons[currentWeaponIndex].currentMagazine--;
				}
			}

			if (weapons[currentWeaponIndex].currentMagazine == 0) {
				isReloading = true;
			}
		}
	}

	void Render() const { DrawCircleV(position, radius, BLUE); }

	void FireWeapon(std::vector<Bullet> &bullets, Vector2 startPos,
					Vector2 targetPos) const {
		for (int i = 0; i < weapons[currentWeaponIndex].bullets; i++) {
			float angle =
				atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);

			float spread = weapons[currentWeaponIndex].spread;

			float spreadOffset = GetRandomValue(-100, 100) / 100.0F * spread;

			angle += spreadOffset * DEG2RAD;

			float dirX = cosf(angle);
			float dirY = sinf(angle);

			bullets.emplace_back(startPos, dirX, dirY,
								 weapons[currentWeaponIndex].damage,
								 weapons[currentWeaponIndex].pierce);
		}
	}

	void SwitchWeapon(int newIndex) {
		if (isReloading) {
			isReloading = false;
		}

		currentWeaponIndex = newIndex;
	}
};
