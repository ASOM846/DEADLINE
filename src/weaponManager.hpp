#pragma once

#include "entity/bullet.hpp"
#include "raylib.h"
#include "screenShake.hpp"
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

	float shakeIntensity{};
};

struct WeaponManager {
	std::vector<Weapon> templates;
	std::vector<Weapon> inventory;

	int currentWeaponIndex = 0;
	int maxSlots = 2;

	float shootTimer{0.0F};
	float reloadTimer{0.0F};

	bool isReloading{false};

	void Init();
	void Reset();

	Weapon *GetCurrentWeapon();

	void SwitchWeaponNext();
	void GiveWeapon(WeaponType newWeapon);
	Weapon *GetWeaponTemplate(WeaponType weaponType);

	void StartReload();
	void Update(std::vector<Bullet> &bullets, Vector2 startPos,
				Vector2 targetPos, ScreenShake &screenShake,
				bool hasPotionRapidFire);

	static std::string GetName(WeaponType type);

  private:
	void FireWeapon(std::vector<Bullet> &bullets, Vector2 startPos,
					Vector2 targetPos, Weapon *w) const;

	void InitTemplate();
};
