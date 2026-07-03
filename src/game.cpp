#include "game.hpp"
#include <raylib.h>
#include <string>

void Game::Init() {
	Reset();
	levelMap.Init();
	player.Init();

	player.position = levelMap.playerSpawnPos;

	camera.target = player.position;
	camera.offset = {static_cast<float>(GetScreenWidth() / 2),
					 static_cast<float>(GetScreenHeight() / 2)};

	camera.rotation = 0.0f;
	camera.zoom = 1.5f;

	waveManager.StartNextWave();
}

void Game::Reset() {
	zombies.clear();
	bullets.clear();
}

void Game::Update() {
	if (IsKeyPressed(KEY_SPACE)) {
		isFreeCam = !isFreeCam;
	}

	if (IsKeyPressed(KEY_P)) {
		isPaused = !isPaused;
	}

	Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), camera);

	levelMap.Update(player.position);
	waveManager.Update(zombies, levelMap);

	zombieManager.ResolveZombieCollision(zombies);

	if (!isFreeCam) {
		camera.zoom = 1.5;
		camera.target = player.position;
	} else {
		camera.zoom = 0.5;
		camera.target = freeCamPos;
	}

	if (IsKeyDown(KEY_RIGHT))
		freeCamPos.x += 10;

	if (IsKeyDown(KEY_LEFT))
		freeCamPos.x -= 10;

	if (IsKeyDown(KEY_UP))
		freeCamPos.y -= 10;

	if (IsKeyDown(KEY_DOWN))
		freeCamPos.y += 10;

	if (IsWindowResized()) {
		camera.offset = {static_cast<float>(GetScreenWidth() / 2),
						 static_cast<float>(GetScreenHeight() / 2)};
	}

	player.Update(bullets, worldMousePos, levelMap);

	collisionManager.ResolvePlayerZombie(player, zombies);
	collisionManager.ResolvePlayerWall(player, levelMap);
	collisionManager.ResolveBulletZombie(bullets, zombies, player);
	collisionManager.ResolveBulletWall(bullets, levelMap);
	collisionManager.ResolvePlayerWeaponSpawner(player, levelMap);
	collisionManager.ResolvePlayerDoor(player, levelMap);

	if (!isPaused)
		zombieManager.UpdateAll(zombies, levelMap);

	bulletManager.UpdateAll(bullets);
}

void Game::Render() {
	BeginMode2D(camera);

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
