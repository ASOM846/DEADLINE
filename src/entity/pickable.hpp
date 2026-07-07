#pragma once

#include "../textureManager.hpp"

#include <algorithm>
#include <raylib.h>
#include <vector>

enum class PickableType {
	HP,
	AMMO,
	COUNT
};

struct Pickable {
	Vector2 position{};
	int startRadius{20};
	int currentRadius{};
	PickableType type{};

	float aliveTime{0.0F};
	float maxAliveTime{5.0F};

	bool alive{true};

	void Update() {
		aliveTime += GetFrameTime();

		float lifeRemaining = maxAliveTime - aliveTime;

		currentRadius = startRadius * lifeRemaining / maxAliveTime;

		if (aliveTime >= maxAliveTime) {
			alive = false;
		}
	}

	void Render(TextureManager &tm) {
		TextureId id;

		switch (type) {
		case PickableType::HP:
			id = TextureId::PICKUP_HP;
			break;
		case PickableType::AMMO:
			id = TextureId::PICKUP_AMMO;
			break;
		}

		// DrawCircleV(position, currentRadius, GREEN);

		Texture2D tex = tm.get(id);

		Rectangle dst = {static_cast<float>(position.x - currentRadius * 1.5),
						 static_cast<float>(position.y - currentRadius * 1.5),
						 static_cast<float>(currentRadius * 3),
						 static_cast<float>(currentRadius * 3)};

		Rectangle src = {0, 0, static_cast<float>(tex.width),
						 static_cast<float>(tex.height)};

		DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, RAYWHITE);
	}
};

class PickableManager {
  public:
	void TrySpawnDrop(std::vector<Pickable> &pickables, Vector2 position,
					  int dropChance) {
		if (GetRandomValue(0, 100) > dropChance)
			return;

		int type = GetRandomValue(0, static_cast<int>(PickableType::COUNT) - 1);
		auto pType = static_cast<PickableType>(type);

		pickables.push_back({
			.position = position,
			.type = pType,
		});

		TraceLog(LOG_INFO, "GENERATED");
	}

	void UpdateAll(std::vector<Pickable> &pickables) {
		for (auto &p : pickables) {
			if (!p.alive) {
				continue;
			}

			p.Update();
		}

		pickables.erase(
			std::remove_if(pickables.begin(), pickables.end(),
						   [](const Pickable &p) { return !p.alive; }),
			pickables.end());
	}

	static void Render(std::vector<Pickable> &pickables, TextureManager &tm) {
		for (auto &p : pickables) {
			p.Render(tm);
		}
	}
};
