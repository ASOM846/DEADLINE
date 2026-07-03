#pragma once

#include <raylib.h>

enum class CameraSetting {
	TAEGET,
	FREE,
};

class CameraManager {
  private:
	Camera2D camera;

	Vector2 freeCamPos{0, 0};

	CameraSetting currentSetting{CameraSetting::TAEGET};

  public:
	Camera2D &GetCamera() { return camera; }

	void Init() {
		camera.rotation = 0.0f;
		camera.zoom = 1.5f;

		camera.offset = {static_cast<float>(GetScreenWidth() / 2),
						 static_cast<float>(GetScreenHeight() / 2)};
	}

	void Update(Vector2 targetPos) {

		if (IsKeyPressed(KEY_UP)) {
			currentSetting = CameraSetting::TAEGET;
			camera.zoom = 1.5f;
		}

		if (IsKeyPressed(KEY_DOWN)) {
			currentSetting = CameraSetting::FREE;
			camera.zoom = 0.5f;
		}

		if (IsWindowResized()) {
			camera.offset = {static_cast<float>(GetScreenWidth() / 2),
							 static_cast<float>(GetScreenHeight() / 2)};
		}

		if (currentSetting == CameraSetting::TAEGET) {
			camera.target = targetPos;
		}

		if (currentSetting == CameraSetting::FREE) {
			if (IsKeyDown(KEY_RIGHT))
				freeCamPos.x += 10;

			if (IsKeyDown(KEY_LEFT))
				freeCamPos.x -= 10;

			if (IsKeyDown(KEY_UP))
				freeCamPos.y -= 10;

			if (IsKeyDown(KEY_DOWN))
				freeCamPos.y += 10;
		}
	}
};
