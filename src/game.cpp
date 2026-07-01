#include "game.hpp"
#include <raylib.h>
#include <string>

void Game::Init() {
	Reset();
	levelMap.Init();
	player.Init();

	player.position.x = levelMap.width * levelMap.cellSize / 2;
	player.position.y = levelMap.height * levelMap.cellSize / 2;

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
		isPaused = !isPaused;
	}

	if (isPaused)
		return;

	Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), camera);

	levelMap.Update(player.position);
	waveManager.Update(zombies, levelMap);

	zombieManager.ResolveZombieCollision(zombies);

	camera.target = player.position;

	player.Update(bullets, worldMousePos);

	collisionManager.ResolvePlayerZombie(player, zombies);
	collisionManager.ResolvePlayerWall(player, levelMap);
	collisionManager.ResolveBulletZombie(bullets, zombies);
	collisionManager.ResolveBulletWall(bullets, levelMap);
	collisionManager.ResolvePlayerWeaponSpawner(player, levelMap);

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
			(player.weaponManager.isReloading ? "REALODING......" : "");

		DrawText(message.c_str(), 100, 10, 30, RED);
	}

	DrawFPS(10, 10);
}
