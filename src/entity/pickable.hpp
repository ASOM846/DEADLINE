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
	int radius{20};
	PickableType type{};

	float aliveTime{0.0f};
	float maxAliveTime{0.0f};

	bool alive{true};

	void Update() {
		aliveTime += GetFrameTime();

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
				p.Update();
			}
		}

		pickables.erase(
			std::remove_if(pickables.begin(), pickables.end(),
						   [](const Pickable &p) { return !p.alive; }),
			pickables.end());
	}

	void Render(std::vector<Pickable> &pickables) {
		for (auto &p : pickables) {
			DrawCircleV(p.position, p.radius, GOLD);
		}
	}
};
