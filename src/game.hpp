#pragma once

#include "audioManager.hpp"
#include "cameraManager.hpp"
#include "collisionManager.hpp"
#include "entity/bullet.hpp"
#include "entity/pickable.hpp"
#include "entity/player.hpp"
#include "entity/zombie.hpp"
#include "floatingText.hpp"
#include "gameStateManager.hpp"
#include "levelMap.hpp"
#include "screenShake.hpp"
#include "textureManager.hpp"
#include "ui.hpp"
#include "waveManager.hpp"
#include <raylib.h>
#include <vector>

class Game {
  public:
	Game() = default;
	~Game() = default;

	void Init();

	void LoadMap(LevelMap map);

	AudioManager &GetAudioManager() { return audioManager; }

	void Reset();

	void Update();
	void Render();

  private:
	void UpdatePlaying();
	void UpdateLost();
	void UpdatePaused();

	void RenderPlaying();
	void RenderLost();
	void RenderPaused();

	GameStateManager gameStateManager;

	TextureManager textureManager;
	AudioManager audioManager;

	Player player;

	WaveManager waveManager;

	CameraManager cameraManager;
	ScreenShake screenShake;
	EffectManager effectManager;

	UI ui;
	BulletManager bulletManager;
	ZombieManager zombieManager;
	PickableManager pickableManager;
	CollisionManager collisionManager;

	LevelMap levelMap;

	std::vector<Zombie> zombies;
	std::vector<Bullet> bullets;
	std::vector<Pickable> pickables;

	friend class WindowManager;
};
