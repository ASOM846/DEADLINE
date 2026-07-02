#pragma once

#include "entity/player.hpp"
#include "weaponManager.hpp"
#include <raylib.h>

class UI {
  public:
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
