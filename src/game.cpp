#include "game.hpp"
#include "entity/pickable.hpp"
#include <raylib.h>

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
	pickables.clear();
}

void Game::Update() {
	Vector2 worldMousePos =
		GetScreenToWorld2D(GetMousePosition(), cameraManager.GetCamera());

	for (auto &rs : levelMap.randomWeaponSpawners) {
		rs.Update();
	}

	levelMap.Update(player.position);
	waveManager.Update(zombies, levelMap);

	zombieManager.ResolveZombieCollision(zombies);
	pickableManager.UpdateAll(pickables);

	player.Update(bullets, worldMousePos, levelMap);

	bulletManager.UpdateAll(bullets);

	zombieManager.UpdateAll(zombies, levelMap);

	collisionManager.ResolvePlayerZombie(player, zombies);
	collisionManager.ResolvePlayerWall(player, levelMap);
	collisionManager.ResolveBulletZombie(bullets, zombies, player, pickables,
										 pickableManager);
	collisionManager.ResolvePlayerKnife(player, zombies, pickableManager,
										pickables);
	collisionManager.ResolveBulletWall(bullets, levelMap);
	collisionManager.ResolvePlayerWeaponSpawner(player, levelMap);
	collisionManager.ResolvePlayerRandomWeaponSpawner(player, levelMap);
	collisionManager.ResolvePlayerDoor(player, levelMap);
	collisionManager.ResolvePlayerPickable(player, pickables);
	collisionManager.ResolvePlayerBlockade(player, levelMap);

	collisionManager.ResolveZombieBlockade(zombies, levelMap);

	cameraManager.Update(player.position);

	if (IsKeyPressed(KEY_P)) {
		pickables.emplace_back(
			Pickable{.position = worldMousePos, .type = PickableType::AMMO});
	}
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

	PickableManager::Render(pickables);

	EndMode2D();

	ui.Render(player, waveManager);

	DrawFPS(10, 10);
}
