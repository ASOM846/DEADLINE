#include "game.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
	levelMap.Init();

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
	levelMap.Update(player.position);
	waveManager.Update(zombies, levelMap);

	zombieManager.ResolveZombieCollision(zombies);

	camera.target = player.position;

	player.Update();

	collisionManager.ResolvePlayerWall(player, levelMap);
	collisionManager.ResolveBulletZombie(bullets, zombies);

	zombieManager.UpdateAll(zombies, levelMap);

	for (auto &b : bullets) {
		b.Update();
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), camera);
		bullets.emplace_back(player.position, worldMousePos);
	}
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

	DrawFPS(10, 10);
}
