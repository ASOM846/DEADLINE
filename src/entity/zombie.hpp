#include "raylib.h"
#include <cmath>

struct Zombie {
	Vector2 position;

	const int width{40};
	const int speed{5};

	void Update(Vector2 targetPos) {
		float dirX = targetPos.x - position.x;
		float dirY = targetPos.y - position.y;

		float distance = std::sqrt(dirX * dirX + dirY * dirY);

		if (distance > 0.0F) {
			position.x += (dirX / distance) * speed;
			position.y += (dirY / distance) * speed;
		}
	}

	void Render() const { DrawCircleV(position, width / 2, GREEN); }
};
