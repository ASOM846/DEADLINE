#pragma once

#include "weaponManager.hpp"
#include <raylib.h>

enum class RandomWeaponSpawnerState {
	IDLE,
	ROLLING,
	DRAWN,
};

struct RandomWeaponSpawner {
  public:
	int pos{};

	WeaponType drawnType{};
	WeaponType tempType{};

	RandomWeaponSpawnerState state;

	std::string message;

	float rollingTimer{};
	float rollingTime{3.0F};

	float textFlashTimer{};
	float textFlashTime{0.25F};

	void Update() {
		switch (state) {
		case RandomWeaponSpawnerState::IDLE:
			return;
		case RandomWeaponSpawnerState::ROLLING:
			rollingTimer += GetFrameTime();
			textFlashTimer += GetFrameTime();

			if (textFlashTimer >= textFlashTime) {
				drawnType = static_cast<WeaponType>(
					GetRandomValue(0, static_cast<int>(WeaponType::COUNT) - 1));
				textFlashTimer = 0.0F;
			}

			if (rollingTimer >= rollingTime) {
				rollingTimer = 0.0F;
				state = RandomWeaponSpawnerState::DRAWN;

				drawnType = static_cast<WeaponType>(
					GetRandomValue(0, static_cast<int>(WeaponType::COUNT) - 1));
			}
			break;
		case RandomWeaponSpawnerState::DRAWN:
			break;
		}
	}

	void Decline() {
		state = RandomWeaponSpawnerState::IDLE;
		rollingTimer = 0.0F;
		textFlashTimer = 0.0F;
	}
};
