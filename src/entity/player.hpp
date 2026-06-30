#pragma once

#include <raylib.h>

struct Player {
	Vector2 position;
	const int size{20};
	const int speed{7};

	void Update() {
		if (IsKeyDown(KEY_W)) {
			position.y -= speed;
		}

		if (IsKeyDown(KEY_S)) {
			position.y += speed;
		}

		if (IsKeyDown(KEY_A)) {
			position.x -= speed;
		}

		if (IsKeyDown(KEY_D)) {
			position.x += speed;
		}
	}

	void Render() const { DrawCircleV(position, size / 2, BLUE); }
};
