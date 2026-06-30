#include "game.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
	levelMap.Init();
	player.Init();

	player.position.x = GetScreenWidth() / 2;
	player.position.y = GetScreenHeight() / 2;

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

	collisionManager.ResolvePlayerWall(player, levelMap);
	collisionManager.ResolveBulletZombie(bullets, zombies);
	collisionManager.ResolveBulletWall(bullets, levelMap);

	zombieManager.UpdateAll(zombies, levelMap);

	bulletManager.UpdateAll(bullets);

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), camera);
		zombies.push_back(Zombie{.position = worldMousePos});
	}
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

	if (player.currentWeapon != nullptr) {
		const char *text = player.currentWeapon->name.c_str();

		DrawText(text, 100, 10, 30, BLUE);
	}

	DrawFPS(10, 10);
}
