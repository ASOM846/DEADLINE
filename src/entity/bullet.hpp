#pragma once

#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <vector>

struct Bullet {
	Vector2 position;
	float dirX{0.0F};
	float dirY{0.0F};

	int speed{20};
	int radius = 5;
	int knockbackForce = 20;
	int damage{};
	int pierce;
	bool alive{true};

	Bullet(Vector2 startPos, Vector2 targetPos, int damage = 20)
		: damage(damage) {
		position = startPos;

		dirX = targetPos.x - startPos.x;
		dirY = targetPos.y - startPos.y;
		float dist = std::sqrt(dirX * dirX + dirY * dirY);

		if (dist > 0.0f) {
			dirX = dirX / dist;
			dirY = dirY / dist;
		}
	}

	Bullet(Vector2 startPos, float dirX, float dirY, int damage = 20,
		   float pierce = 0)
		: position(startPos), dirX(dirX), dirY(dirY), damage(damage),
		  pierce(pierce) {}

	void Update() {
		position.x += dirX * speed;
		position.y += dirY * speed;
	}
	void Render() const { DrawCircleV(position, radius, RED); }
};

class BulletManager {
  public:
	void UpdateAll(std::vector<Bullet> &bullets) {
		for (auto &b : bullets) {
			if (!b.alive)
				continue;
			b.Update();
		}

		bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
									 [](const Bullet &b) { return !b.alive; }),
					  bullets.end());
	}
};
