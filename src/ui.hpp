#pragma once

#include "baricade.hpp"
#include "entity/player.hpp"
#include "randomWeapon.hpp"
#include "waveManager.hpp"
#include "weaponManager.hpp"
#include <raylib.h>
#include <string>

class UI {
  public:
	void Render(Player &player, WaveManager &waveManager) {
		DrawPlayerStats(player);

		DrawWaveInfo(waveManager);

		if (player.currentDoor != nullptr) {
			DrawDoorInfo(player.currentDoor);
		}

		if (player.currentBlockade != nullptr) {
			DrawBlockadeInfo(player.currentBlockade);
		}

		if (player.currentSpawner != nullptr &&
			player.spawnerWeapon != nullptr) {
			DrawWeaponShopInfo(player);
		}

		if (player.currentRandomSpawner != nullptr) {
			DrawRandomWeaponInfo(player.currentRandomSpawner);
		}
	}

  private:
	void DrawPlayerStats(Player &player) {
		Weapon *currentWeapon = player.weaponManager.GetCurrentWeapon();

		if (currentWeapon != nullptr) {
			std::string message =
				currentWeapon->name + "     " +
				std::to_string(currentWeapon->currentMagazine) + "/" +
				std::to_string(currentWeapon->maxMagazine) + "       " +
				std::to_string(currentWeapon->ammo) + "    " +
				(player.weaponManager.isReloading ? "REALODING......" : "") +
				"     " + std::to_string(player.money);

			DrawText(message.c_str(), 100, 10, 30, BLACK);
		}

		float hp = player.hp;
		float maxHp = player.maxHp;

		int barWidth = 200;

		float percnt = hp / maxHp * barWidth;

		DrawRectangleLines(50, GetScreenHeight() - 50, barWidth, 40, LIGHTGRAY);
		DrawRectangle(50, GetScreenHeight() - 50, percnt, 40, GREEN);
	}

	void DrawWaveInfo(WaveManager &waveManager) {
		std::string text;

		text = "WAVE:   " + std::to_string(waveManager.wave) + "   " +
			   std::to_string(waveManager.currentZombieHp);

		DrawText(text.c_str(), 10, 100, 30, GREEN);
	}

	void DrawDoorInfo(Door *currentDoor) {
		std::string text;

		text = "OPEN DOOR:  " + std::to_string(currentDoor->price) + "   $";

		int fontSize = 30;
		int textWidth = MeasureText(text.c_str(), fontSize);

		DrawText(text.c_str(), (GetScreenWidth() - textWidth) / 2,
				 GetScreenHeight() / 2, fontSize, BLACK);
	}

	void DrawBlockadeInfo(Blockade *currentBlockade) {
		std::string text;

		int hp = static_cast<int>(currentBlockade->hp);

		text = "BLOCKADE HP:  " + std::to_string(hp);

		int fontSize = 30;
		int textWidth = MeasureText(text.c_str(), fontSize);

		DrawText(text.c_str(), (GetScreenWidth() - textWidth) / 2,
				 GetScreenHeight() / 2, fontSize, BLACK);
	}

	void DrawWeaponShopInfo(Player &player) {
		if (player.currentSpawner == nullptr ||
			player.spawnerWeapon == nullptr) {
			return;
		}

		WeaponSpawner *ws = player.currentSpawner;
		Weapon *shopW = player.spawnerWeapon;

		if (ws == nullptr)
			return;

		std::string text;
		switch (ws->type) {
		case WeaponType::PISTOL:
			text = "PISTOL";
			break;
		case WeaponType::UZI:
			text = "UZI";
			break;
		case WeaponType::AK47:
			text = "AK47";
			break;
		case WeaponType::SNIPER:
			text = "SNIPER";
			break;
		case WeaponType::SHOTGUN:
			text = "SHOTGUN";
			break;
		case WeaponType::COUNT:
			break;
		}

		text += "     ";

		std::string text2;
		Weapon *w = player.weaponManager.GetCurrentWeapon();

		if (w->type == ws->type) {
			text2 = "MAG PRICE:   " + std::to_string(shopW->magazinePrice);
		}

		if (w->type != ws->type) {
			text2 = "WEAPON PRICE:   " + std::to_string(shopW->price);
		}

		text += text2;

		int fontSize = 30;
		int textWidth = MeasureText(text.c_str(), fontSize);

		DrawText(text.c_str(), (GetScreenWidth() - textWidth) / 2,
				 GetScreenHeight() / 2, fontSize, BLACK);
	}

	void DrawRandomWeaponInfo(RandomWeaponSpawner *spawner) {
		if (spawner == nullptr)
			return;

		std::string text;

		switch (spawner->state) {
		case RandomWeaponSpawnerState::IDLE:
			text = "PRESS `E` TO DRAW";
			break;
		case RandomWeaponSpawnerState::ROLLING:
			text = WeaponManager::GetName(spawner->drawnType);
			break;
		case RandomWeaponSpawnerState::DRAWN:
			int remainingTime = spawner->resetTime - spawner->resetTimer;

			text = WeaponManager::GetName(spawner->drawnType);
			text += "\n";
			text += std::to_string(remainingTime);
			break;
		}

		int fontSize = 30;
		int textWidth = MeasureText(text.c_str(), fontSize);

		DrawText(text.c_str(), (GetScreenWidth() - textWidth) / 2,
				 GetScreenHeight() / 2, fontSize, BLACK);
	}
};
