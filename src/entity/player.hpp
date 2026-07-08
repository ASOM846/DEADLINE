#pragma once

#include "../floatingText.hpp"
#include "../levelMap.hpp"
#include "../screenShake.hpp"
#include "../textureManager.hpp"
#include "../weaponManager.hpp"
#include "bullet.hpp"
#include "pickable.hpp"
#include <raylib.h>
#include <vector>

struct Player {
	Vector2 position;
	const int radius{20};
	const float visualRadius{23.0f};

	float speed{4.0F};

	float rotation{0.0F};

	const float textureRotation{90.0f};

	float maxHp{100};
	float hp{100};

	int money = 10000;

	Vector2 velocity{0, 0};

	float ammoVisibliltyTimer = 0.0f;

	float knifeCooldownTimer{0.0f};
	const float knifeCooldownDuration{0.5f};
	float knifeVisualTimer{0.0f};
	Vector2 knifeDir{0, 0};
	bool knifeTriggered{false};

	bool hasPotionSpeed{false};
	bool hasPotionThirdSlot{false};
	bool hasPotionRapidFire{false};
	bool hasPotionHeal{false};

	const float knifeRange{75.0f};
	const int knifeDamage{150};

	WeaponManager weaponManager;

	WeaponSpawner *currentSpawner{nullptr};
	RandomWeaponSpawner *currentRandomSpawner{nullptr};
	Weapon *spawnerWeapon{nullptr};
	Door *currentDoor{nullptr};
	Pickable *currentPickable{nullptr};
	Blockade *currentBlockade{nullptr};
	PotionSpawner *currentPotionSpawn{nullptr};

	void Init();

	void Reset();

	void Update(std::vector<Bullet> &bullets, Vector2 worldMousePos,
				LevelMap &map, ScreenShake &screenShake,
				EffectManager &EffectManager);

	void Render(TextureManager &tm);

  private:
	void DrawAmmo(float timer = 2.0f) { ammoVisibliltyTimer = timer; }

	void HandleMovement();
	void HandleKnifeActions(Vector2 worldMousePos);
	void HandleWeaponActions(std::vector<Bullet> &bullets);
	void HandleRandomSpawnerActions();
	void HandleDoorActions(LevelMap &map);
	void HandlePickableActions();
	void HandleBlockadeActions();
	void HandlePotionSpawnActions();

	bool CanBuy(int val);
};
