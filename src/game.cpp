#include "game.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
	player.position.x = GetScreenWidth() / 2;
	player.position.y = GetScreenHeight() / 2;
}

void Game::Reset() {
	zombies.clear();
}

void Game::Update() {
	player.Update();

	for (auto &z : zombies) {
		z.Update(player.position);
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		zombies.push_back(Zombie{.position = GetMousePosition()});
	}
}

void Game::Render() {
	player.Render();

	for (auto &z : zombies) {
		z.Render();
	}
}
