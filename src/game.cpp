#include "game.hpp"
#include "entity/pickable.hpp"
#include "floatingText.hpp"
#include "gameStateManager.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();

	textureManager.loadAll();

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

	effectManager.Reset();
	player.Reset();
}

void Game::Update() {
	switch (gameStateManager.GetCurrentState()) {
	case GameState::PLAYING:
		UpdatePlaying();
		break;
	case GameState::PAUSED:
	case GameState::LOST:
		UpdateLost();
		break;
	}
}

void Game::Render() {
	switch (gameStateManager.GetCurrentState()) {
	case GameState::PLAYING:
		RenderPlaying();
		break;
	case GameState::PAUSED:
	case GameState::LOST:
		RenderLost();
		break;
	}
}

void Game::UpdatePlaying() {
	Vector2 worldMousePos =
		GetScreenToWorld2D(GetMousePosition(), cameraManager.GetCamera());

	for (auto &rs : levelMap.randomWeaponSpawners) {
		rs.Update();
	}

	levelMap.Update(player.position);
	waveManager.Update(zombies, levelMap);

	effectManager.Update(GetFrameTime());

	zombieManager.ResolveZombieCollision(zombies);
	pickableManager.UpdateAll(pickables);

	player.Update(bullets, worldMousePos, levelMap, screenShake, effectManager);

	bulletManager.UpdateAll(bullets);

	zombieManager.UpdateAll(zombies, levelMap, effectManager);

	collisionManager.ResolvePlayerZombie(player, zombies);
	collisionManager.ResolvePlayerWall(player, levelMap);
	collisionManager.ResolveBulletZombie(bullets, zombies, player, pickables,
										 pickableManager);
	collisionManager.ResolvePlayerKnife(player, zombies, pickableManager,
										pickables);

	collisionManager.ResolveBulletWall(bullets, levelMap);
	collisionManager.ResolvePlayerWeaponSpawner(player, levelMap);
	collisionManager.ResolvePlayerRandomWeaponSpawner(player, levelMap);
	collisionManager.ResolvePlayerPotionSpawner(player, levelMap);
	collisionManager.ResolvePlayerDoor(player, levelMap);
	collisionManager.ResolvePlayerPickable(player, pickables);
	collisionManager.ResolvePlayerBlockade(player, levelMap);

	collisionManager.ResolveZombieBlockade(zombies, levelMap);

	if (IsKeyPressed(KEY_X))
		effectManager.SpawnText(worldMousePos, "TEST", RED);

	screenShake.update(GetFrameTime());
	cameraManager.Update(player.position, screenShake.offset);

	if (IsKeyPressed(KEY_P)) {
		pickables.emplace_back(
			Pickable{.position = worldMousePos, .type = PickableType::AMMO});
	}

	gameStateManager.Update(player);
}

void Game::UpdateLost() {
	if (IsKeyPressed(KEY_SPACE)) {
		Init();
		gameStateManager.SwitchState(GameState::PLAYING);
	}
}

void Game::RenderPlaying() {
	BeginMode2D(cameraManager.GetCamera());

	levelMap.Render();

	player.Render(textureManager);

	for (auto &z : zombies) {
		z.Render(textureManager);
	}

	for (auto &b : bullets) {
		b.Render();
	}

	PickableManager::Render(pickables);

	effectManager.Draw();

	EndMode2D();

	ui.Render(player, waveManager);

	DrawFPS(10, 10);
}

void Game::RenderLost() {
	RenderPlaying();
	ui.RenderLost();
}
