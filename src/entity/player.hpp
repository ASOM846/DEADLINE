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

	WeaponManager weaponManager;

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
		weaponManager.inventory.push_back(pistol);

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
		weaponManager.inventory.push_back(uzi);

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
		weaponManager.inventory.push_back(Ak47);

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
		weaponManager.inventory.push_back(sniper);

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
		weaponManager.inventory.push_back(shotgun);
	}

	void Update(std::vector<Bullet> &bullets, Vector2 worldMousePos) {
		velocity = {.x = 0, .y = 0};

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			weaponManager.SwitchWeaponNext();
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
