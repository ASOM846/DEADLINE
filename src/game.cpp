#include "game.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
	levelMap.Init();

	player.position.x = GetScreenWidth() / 2;
	player.position.y = GetScreenHeight() / 2;
}

void Game::Reset() {
	zombies.clear();
}

void Game::Update() {
	levelMap.Update(player.position);
	zombieManager.ResolveZombieCollision(zombies);
	player.Update();

	for (auto &z : zombies) {
		z.Update(levelMap);
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		zombies.push_back(Zombie{.position = GetMousePosition()});
	}
}

void Game::Render() {
	levelMap.Render();

	player.Render();

	for (auto &z : zombies) {
		z.Render();
	}
}
