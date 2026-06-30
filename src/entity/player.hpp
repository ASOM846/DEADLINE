#pragma once

#include "bullet.hpp"
#include <raylib.h>
#include <string>
#include <vector>

struct Weapon {
	Weapon() = default;
	~Weapon() = default;

	std::string name;
	int damage{};
	int bullets{};
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

	Vector2 velocity{0, 0};

	std::vector<Weapon> weapons;

	Weapon *currentWeapon{nullptr};

	void Init() {
		Weapon pistol;
		pistol.name = "Pistol";
		pistol.damage = 20;
		pistol.bullets = 1;
		pistol.spread = 0.0F;
		pistol.fireRate = 1.0f;
		pistol.pierce = 0;
		pistol.isAutomatic = false;
		weapons.push_back(pistol);

		Weapon uzi;
		uzi.name = "UZI";
		uzi.damage = 20;
		uzi.bullets = 1;
		uzi.spread = 10.0;
		uzi.fireRate = 0.2f;
		uzi.pierce = 0;
		uzi.isAutomatic = true;
		weapons.push_back(uzi);

		Weapon sniper;
		sniper.name = "Sniper";
		sniper.bullets = 1;
		sniper.damage = 100;
		sniper.spread = 0.0F;
		sniper.fireRate = 3.0f;
		sniper.pierce = 3;
		sniper.isAutomatic = false;
		weapons.push_back(sniper);

		Weapon shotgun;
		shotgun.name = "Shotgun";
		shotgun.bullets = 5;
		shotgun.damage = 50;
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

			TraceLog(LOG_INFO, std::to_string(shootTimer).c_str());
			if (shootTimer < currentWeapon->fireRate) {
				return;
			}

			if (currentWeapon->isAutomatic) {
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
					FireWeapon(bullets, position, worldMousePos);
					shootTimer = 0.0F;
				}
			} else {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					FireWeapon(bullets, position, worldMousePos);
					shootTimer = 0.0F;
				}
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
