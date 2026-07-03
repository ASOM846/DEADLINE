#pragma once

#include <algorithm>
#include <raylib.h>
#include <vector>

enum class PickableType {
	HP,
	AMMO
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
};

class PickableManager {
  public:
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

	static void Render(std::vector<Pickable> &pickables) {
		for (auto &p : pickables) {
			DrawCircleV(p.position, p.currentRadius, GOLD);
		}
	}
};
