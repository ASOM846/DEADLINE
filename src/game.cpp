#include "game.hpp"
#include <raylib.h>
#include <string>

void Game::Init() {
	Reset();

	levelMap.Init();
	player.Init();

	cameraManager.Init();

	player.position = levelMap.playerSpawnPos;

	waveManager.StartNextWave();
}

void Game::Reset() {
	zombies.clear();
	bullets.clear();
}

void Game::Update() {

	Vector2 worldMousePos =
		GetScreenToWorld2D(GetMousePosition(), cameraManager.GetCamera());

	levelMap.Update(player.position);
	waveManager.Update(zombies, levelMap);

	zombieManager.ResolveZombieCollision(zombies);

	player.Update(bullets, worldMousePos, levelMap);

	collisionManager.ResolvePlayerZombie(player, zombies);
	collisionManager.ResolvePlayerWall(player, levelMap);
	collisionManager.ResolveBulletZombie(bullets, zombies, player);
	collisionManager.ResolveBulletWall(bullets, levelMap);
	collisionManager.ResolvePlayerWeaponSpawner(player, levelMap);
	collisionManager.ResolvePlayerDoor(player, levelMap);

	zombieManager.UpdateAll(zombies, levelMap);

	bulletManager.UpdateAll(bullets);

	cameraManager.Update(player.position);
}

void Game::Render() {
	BeginMode2D(cameraManager.GetCamera());

	levelMap.Render();

	player.Render();

	for (auto &z : zombies) {
		z.Render();
	}

	for (auto &b : bullets) {
		b.Render();
	}

	EndMode2D();

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

	std::string text = std::to_string(zombies.size());

	DrawText(text.c_str(), 100, GetScreenHeight() - 100, 30, BLACK);

	ui.DrawWeaponShopInfo(player);
	ui.DrawDoorInfo(player);

	DrawFPS(10, 10);
}
