#pragma once

#include "bullet.hpp"
#include <numbers>
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

	bool isReloading{false};

	Vector2 velocity{0, 0};

	std::vector<Weapon> weapons;

	Weapon *currentWeapon{nullptr};

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

		currentWeapon = &weapons[0];
	}

	void Update(std::vector<Bullet> &bullets, Vector2 worldMousePos) {
		velocity = {.x = 0, .y = 0};

		if (IsKeyDown(KEY_ONE)) {
			currentWeapon = &weapons[0];
		}

		if (IsKeyDown(KEY_TWO)) {
			currentWeapon = &weapons[1];
		}

		if (IsKeyDown(KEY_THREE)) {
			currentWeapon = &weapons[2];
		}

		if (IsKeyDown(KEY_R)) {
			isReloading = true;
		}

		if (IsKeyDown(KEY_E)) {
			currentWeapon = &weapons[3];
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

		if (currentWeapon != nullptr) {
			shootTimer += GetFrameTime();

			if (isReloading) {
				reloadTimer += GetFrameTime();
			}

			if (currentWeapon->ammo == 0) {
				isReloading = false;
			}

			if (reloadTimer >= currentWeapon->reloadTime) {
				isReloading = false;
				int remainingRounds =
					currentWeapon->maxMagazine - currentWeapon->currentMagazine;

				if (currentWeapon->ammo < remainingRounds) {
					currentWeapon->currentMagazine += currentWeapon->ammo;
					currentWeapon->ammo = 0;
				} else {
					currentWeapon->currentMagazine += remainingRounds;
					currentWeapon->ammo -= remainingRounds;
				}
				reloadTimer = 0.0F;
			}

			if (shootTimer < currentWeapon->fireRate) {
				return;
			}

			if (currentWeapon->isAutomatic && !isReloading &&
				currentWeapon->currentMagazine > 0) {
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
					FireWeapon(bullets, position, worldMousePos);
					shootTimer = 0.0F;
					currentWeapon->currentMagazine--;
				}
			} else {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isReloading &&
					currentWeapon->currentMagazine > 0) {
					FireWeapon(bullets, position, worldMousePos);
					shootTimer = 0.0F;
					currentWeapon->currentMagazine--;
				}
			}

			if (currentWeapon->currentMagazine == 0) {
				isReloading = true;
			}
		}
	}

	void Render() const { DrawCircleV(position, radius, BLUE); }

	void FireWeapon(std::vector<Bullet> &bullets, Vector2 startPos,
					Vector2 targetPos) const {
		for (int i = 0; i < currentWeapon->bullets; i++) {
			float angle =
				atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);

			float spread = currentWeapon->spread;

			float spreadOffset = GetRandomValue(-100, 100) / 100.0F * spread;

			angle += spreadOffset * DEG2RAD;

			float dirX = cosf(angle);
			float dirY = sinf(angle);

			bullets.emplace_back(startPos, dirX, dirY, currentWeapon->damage,
								 currentWeapon->pierce);
		}
	}
};
