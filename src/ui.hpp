#pragma once

#include "entity/player.hpp"
#include "weaponManager.hpp"
#include <raylib.h>

class UI {
  public:
	void DrawWeaponShopInfo(Player &player) {
		WeaponSpawner *ws = player.currentSpawner;

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

		Weapon *temp = player.weaponManager.GetWeapon(ws->type);

		if (w->type == ws->type) {
			text2 = "MAG PRICE:   " + std::to_string(temp->magazinePrice);
		}

		if (w->type != ws->type) {
			text2 = "WEAPON PRICE:   " + std::to_string(temp->price);
		}

		text += text2;

		int fontSize = 30;
		int textWidth = MeasureText(text.c_str(), fontSize);

		DrawText(text.c_str(), (GetScreenWidth() - textWidth) / 2,
				 GetScreenHeight() / 2, fontSize, RED);
	}
};
