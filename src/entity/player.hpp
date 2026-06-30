#pragma once

#include <raylib.h>

struct Player {
	Vector2 position;
	const int radius{25};
	const float speed{5.0F};

	Vector2 velocity{0, 0};

	void Update() {
		velocity = {.x = 0, .y = 0};

		if (IsKeyDown(KEY_W)) {
			velocity.y -= speed;
		}

		if (IsKeyDown(KEY_S)) {
			velocity.y += speed;
		}

		if (IsKeyDown(KEY_A)) {
			velocity.x -= speed;
		}

		if (IsKeyDown(KEY_D)) {
			velocity.x += speed;
		}
	}

	void Render() const { DrawCircleV(position, radius, BLUE); }
};
