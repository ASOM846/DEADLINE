#pragma once

#include "baricade.hpp"
#include "entity/player.hpp"
#include "potion.hpp"
#include "randomWeapon.hpp"
#include "textureManager.hpp"
#include "waveManager.hpp"
#include "weaponManager.hpp"
#include <raylib.h>
#include <string>

class UI {
  public:
	void Render(Player &player, WaveManager &waveManager, TextureManager &tm) {
		// DrawPlayerStats(player);
		//
		// DrawWaveInfo(waveManager);

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

		if (player.currentPotionSpawn != nullptr) {
			DrawPotionSpawnerInfo(player.currentPotionSpawn);
		}

		DrawHPEffect(player, tm);
		DrawMinimalHUD(player, waveManager);

		DrawPotionsHUD(player);
	}

	void RenderLost() {
		int barH = 150;
		DrawRectangle(0, (GetScreenHeight() - barH) / 2, GetScreenWidth(), barH,
					  Fade(BLACK, 0.3f));

		std::string text = "LOST";
		int textSize = 30;

		std::string text2 = "Press SPACE to reset";
		int textSize2 = 20;

		int textW = MeasureText(text.c_str(), textSize);
		int text2W = MeasureText(text2.c_str(), textSize2);

		DrawText(text.c_str(), (GetScreenWidth() - textW) / 2,
				 GetScreenHeight() / 2 - 15, textSize, RED);

		DrawText(text2.c_str(), (GetScreenWidth() - text2W) / 2,
				 GetScreenHeight() / 2 + 15, textSize2, WHITE);
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
				(player.weaponManager.isReloading ? "RELOADING......" : "") +
				"     " + std::to_string(player.money);

			DrawText(message.c_str(), 100, 10, 30, BLACK);
		}

		float hp = player.hp;
		float maxHp = player.maxHp;

		int barWidth = 200;

		float percnt = hp / maxHp * barWidth;

		DrawRectangleLines(50, GetScreenHeight() - 50, barWidth, 40, LIGHTGRAY);
		DrawRectangle(50, GetScreenHeight() - 50, percnt, 40, GREEN);

		int rectW = 40;
		int spacing = 20;

		if (player.hasPotionSpeed) {
			DrawRectangle(GetScreenWidth() - rectW * 4 - spacing * 4,
						  GetScreenHeight() - 60, rectW, rectW, BLUE);
		}

		if (player.hasPotionThirdSlot) {
			DrawRectangle(GetScreenWidth() - rectW * 3 - spacing * 3,
						  GetScreenHeight() - 60, rectW, rectW, GREEN);
		}

		if (player.hasPotionRapidFire) {
			DrawRectangle(GetScreenWidth() - rectW * 2 - spacing * 2,
						  GetScreenHeight() - 60, rectW, rectW, GOLD);
		}

		if (player.hasPotionHeal) {
			DrawRectangle(GetScreenWidth() - rectW * 1 - spacing * 1,
						  GetScreenHeight() - 60, rectW, rectW, RED);
		}
	}

	void DrawHPEffect(Player &player, TextureManager &tm) {
		float hpRatio = player.hp / player.maxHp;

		Texture2D tex = tm.get(TextureId::EFFECT_VINETE2);

		Rectangle dst = {0, 0, static_cast<float>(GetScreenWidth()),
						 static_cast<float>(GetScreenHeight())};

		Rectangle src = {0, 0, static_cast<float>(tex.width),
						 static_cast<float>(tex.height)};

		DrawTexturePro(tex, src, dst, {0, 0}, 0.0F, RAYWHITE);

		if (hpRatio >= 1.0f) {
			return;
		}

		float baseAlpha = (1.0f - hpRatio) * 0.4f;

		if (hpRatio < 0.3f) {
			float pulse = (std::sin(GetTime() * 8.0f) + 1.0f) / 2.0f;
			baseAlpha += pulse * 0.2;
		}

		tex = tm.get(TextureId::EFFECT_VINETE);

		dst = {0, 0, static_cast<float>(GetScreenWidth()),
			   static_cast<float>(GetScreenHeight())};

		src = {0, 0, static_cast<float>(tex.width),
			   static_cast<float>(tex.height)};

		Color tint = Fade(RED, baseAlpha);

		DrawTexturePro(tex, src, dst, {0, 0}, 0.0F, tint);
	}

	void DrawMinimalHUD(Player &player, WaveManager &waveManager) {
		int fontSize = 40;
		std::string waveText = std::to_string(waveManager.wave);

		DrawText(waveText.c_str(), 22, 22, fontSize, Fade(BLACK, 0.5f));
		DrawText(waveText.c_str(), 20, 20, fontSize, WHITE);

		std::string moneyText = "$" + std::to_string(player.money);
		int textWidth = MeasureText(moneyText.c_str(), fontSize);

		int posX = GetScreenWidth() - textWidth - 30;
		int posY = GetScreenHeight() - fontSize - 30;

		DrawText(moneyText.c_str(), posX + 2, posY + 2, fontSize,
				 Fade(BLACK, 0.5F));
		DrawText(moneyText.c_str(), posX, posY, fontSize, MAROON);
	}

	void DrawPotionsHUD(Player &player) {
		float startX = 30.0f;
		float boxSize = 24.0f;
		float spacing = 10.0f;
		float startY = GetScreenHeight() - boxSize - 30.0f;

		struct PotionData {
			bool hasIt;
			Color col;
		};

		PotionData potionList[] = {{player.hasPotionSpeed, BLUE},
								   {player.hasPotionThirdSlot, GREEN},
								   {player.hasPotionRapidFire, GOLD},
								   {player.hasPotionHeal, RED}};

		for (int i = 0; i < 4; i++) {
			float currentX = startX + i * (boxSize + spacing);
			Rectangle rect = {currentX, startY, boxSize, boxSize};

			if (potionList[i].hasIt) {
				DrawRectangleRec(rect, Fade(potionList[i].col, 0.7f));
				DrawRectangleLinesEx(rect, 1.5f, WHITE);
			} else {
				DrawRectangleRec(rect, Fade(BLACK, 0.15f));
				DrawRectangleLinesEx(rect, 1.0f, Fade(WHITE, 0.2f));
			}
		}
	}

	void DrawDoorInfo(Door *currentDoor) {
		std::string text;

		text = "OPEN DOOR:  " + std::to_string(currentDoor->price) + "   $";

		int fontSize = 30;
		int textWidth = MeasureText(text.c_str(), fontSize);

		DrawText(text.c_str(), (GetScreenWidth() - textWidth) / 2,
				 GetScreenHeight() / 3, fontSize, BLACK);
	}

	void DrawBlockadeInfo(Blockade *currentBlockade) {
		std::string text;

		int hp = static_cast<int>(currentBlockade->hp);

		text = "BLOCKADE HP:  " + std::to_string(hp);

		int fontSize = 30;
		int textWidth = MeasureText(text.c_str(), fontSize);

		DrawText(text.c_str(), (GetScreenWidth() - textWidth) / 2,
				 GetScreenHeight() / 3, fontSize, BLACK);
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
		case WeaponType::DOUBLE_BARREL:
			text = "DOUBLE_BARREL";
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
				 GetScreenHeight() / 3, fontSize, BLACK);
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
				 GetScreenHeight() / 3, fontSize, BLACK);
	}

	void DrawPotionSpawnerInfo(PotionSpawner *spawner) {
		if (spawner == nullptr)
			return;

		std::string text;

		switch (spawner->type) {
		case PotionType::SPEED:
			text = "POTION SPEED";
			break;
		case PotionType::THIRD_SLOT:
			text = "THIRD SLOT";
			break;
		case PotionType::RAPID_FIRE:
			text = "RAPID FIRE";
			break;
		case PotionType::HEAL:
			text = "HEAL";
			break;
		}

		int fontSize = 30;
		int textWidth = MeasureText(text.c_str(), fontSize);

		DrawText(text.c_str(), (GetScreenWidth() - textWidth) / 2,
				 GetScreenHeight() / 3, fontSize, BLACK);
	}
};
