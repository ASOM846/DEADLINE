#pragma once

#include "../weaponManager.hpp"
#include "bullet.hpp"
#include <raylib.h>
#include <vector>

struct Player {
	Vector2 position;
	const int radius{25};
	const float speed{5.0F};

	float hp{100};

	Vector2 velocity{0, 0};

	WeaponSpawner *currentSpawner{nullptr};

	WeaponManager weaponManager;

	void Init() { weaponManager.Init(); }

	void Update(std::vector<Bullet> &bullets, Vector2 worldMousePos) {
		velocity = {.x = 0, .y = 0};

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			weaponManager.SwitchWeaponNext();
		}

		if (IsKeyPressed(KEY_E) && currentSpawner != nullptr) {
			Weapon *w = weaponManager.GetCurrentWeapon();
			if (w != nullptr) {
				w->ammo += 40;
			}
		}

		if (IsKeyDown(KEY_R)) {
			weaponManager.StartReload();
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

		weaponManager.Update(bullets, position, worldMousePos);
	}

	void Render() const { DrawCircleV(position, radius, BLUE); }
};
