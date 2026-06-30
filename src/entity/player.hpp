#pragma once

#include "bullet.hpp"
#include <raylib.h>
#include <string>
#include <vector>

struct Weapon {
	Weapon() = default;
	~Weapon() = default;

	std::string name{};
	int damage{};
	float fireRate{};
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
		pistol.fireRate = 1.0f;

		weapons.push_back(pistol);
	}

	void Update(std::vector<Bullet> &bullets, Vector2 worldMousePos) {
		currentWeapon = &weapons.front();

		velocity = {.x = 0, .y = 0};

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
			if (shootTimer <= currentWeapon->fireRate) {
				return;
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				bullets.emplace_back(position, worldMousePos,
									 currentWeapon->damage);
				shootTimer = 0.0F;
			}
		}
	}

	void Render() const { DrawCircleV(position, radius, BLUE); }
};
