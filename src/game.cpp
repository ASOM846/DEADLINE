#include "game.hpp"
#include "audioManager.hpp"
#include "entity/pickable.hpp"
#include "floatingText.hpp"
#include "gameStateManager.hpp"
#include "levelMap.hpp"
#include <raylib.h>

#include <utility>

void Game::Init() {
	Reset();

	textureManager.loadAll();
	audioManager.loadAll();

	player.Init();

	cameraManager.Init();

	audioManager.PlayMusic(MusicId::BCG_MUSIC);
}

void Game::LoadMap(LevelMap map) {
	levelMap = std::move(map);
	levelMap.Init();
	player.position = levelMap.playerSpawnPos;
	waveManager.StartNextWave();
	gameStateManager.SwitchState(GameState::PLAYING);
}

void Game::Reset() {
	zombies.clear();
	bullets.clear();
	pickables.clear();

	effectManager.Reset();
	player.Reset();

	// audioManager.ResetMusic(MusicId::BCG_MUSIC);
}

void Game::Update() {
	switch (gameStateManager.GetCurrentState()) {
	case GameState::PLAYING:
		UpdatePlaying();
		break;
	case GameState::PAUSED:
		UpdatePaused();
		break;
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
		RenderPaused();
		break;
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

	player.Update(bullets, worldMousePos, levelMap, screenShake, effectManager,
				  audioManager);

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

	if (IsKeyDown(KEY_X))
		player.hp -= GetFrameTime() * 10;

	screenShake.update(GetFrameTime());
	cameraManager.Update(player.position, screenShake.offset);

	if (IsKeyDown(KEY_P)) {
		player.hp += GetFrameTime() * 10;
	}

	gameStateManager.Update(player);
}

void Game::UpdateLost() {
	if (IsKeyPressed(KEY_SPACE)) {
		Init();
		gameStateManager.SwitchState(GameState::RETURN_TO_MENU);
	}
}

void Game::UpdatePaused() {
	if (IsKeyPressed(KEY_X))
		gameStateManager.SwitchState(GameState::PLAYING);
};

void Game::RenderPlaying() {
	BeginMode2D(cameraManager.GetCamera());

	levelMap.Render(textureManager);

	player.Render(textureManager);

	for (auto &z : zombies) {
		z.Render(textureManager);
	}

	for (auto &b : bullets) {
		b.Render();
	}

	PickableManager::Render(pickables, textureManager);

	effectManager.Draw();

	EndMode2D();

	ui.Render(player, waveManager, textureManager);

	DrawFPS(10, 10);
}

void Game::RenderLost() {
	RenderPlaying();
	ui.RenderLost();
}

void Game::RenderPaused() {
	RenderPlaying();
}
