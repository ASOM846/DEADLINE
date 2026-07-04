#pragma once

#include "baricade.hpp"
#include "entity/player.hpp"
#include "weaponManager.hpp"
#include <raylib.h>
#include <string>

class UI {
  public:
	void Render(Player &player) {
		DrawPlayerStats(player);

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
};
