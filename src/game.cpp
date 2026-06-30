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
	camera.zoom = 1.0f;
}

void Game::Reset() {
	zombies.clear();
}

void Game::Update() {
	levelMap.Update(player.position);
	zombieManager.ResolveZombieCollision(zombies);

	camera.target = player.position;

	player.Update();

	for (auto &z : zombies) {
		z.Update(levelMap);
	}
	BeginMode2D(camera);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		zombies.push_back(Zombie{.position = GetMousePosition()});
	}
	EndMode2D();
}

void Game::Render() {
	BeginMode2D(camera);

	levelMap.Render();

	player.Render();

	for (auto &z : zombies) {
		z.Render();
	}

	EndMode2D();
}
