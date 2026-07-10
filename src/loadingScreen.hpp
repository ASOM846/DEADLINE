#pragma once

#include <cmath>
#include <raylib.h>
enum class LoadingState {
	LOADING,
	READY,
};

class LoadingScreen {

	float loadingTimer{0.0f};
	float minLoadingTime{3.0f};

  public:
	LoadingState currentState{LoadingState::LOADING};

	void Reset() {
		loadingTimer = 0.0f;
		currentState = LoadingState::LOADING;
	}

	void Update() {
		loadingTimer += GetFrameTime();

		switch (currentState) {
		case LoadingState::LOADING:
			if (loadingTimer >= minLoadingTime) {
				currentState = LoadingState::READY;
			}
			break;
		case LoadingState::READY:
			break;
		}
	}

	void Render() {
		float visualScreenWidth =
			(GetScreenWidth() > 1600) ? 1600 : GetScreenWidth();
		float visualScreenHeight =
			(GetScreenHeight() > 900) ? 900 : GetScreenHeight();

		Vector2 offset;

		offset.x = (GetScreenWidth() - visualScreenWidth) / 2.0f;
		offset.y = (GetScreenHeight() - visualScreenHeight) / 2.0f;

		float margin = 40.0f;
		float wheelRadius = 20.0f;

		float wheelCX = static_cast<float>(GetScreenWidth()) - offset.x -
						margin - wheelRadius;
		float wheelCY = static_cast<float>(GetScreenHeight()) - offset.y -
						margin - wheelRadius;

		RenderLoadingWheel(wheelCX, wheelCY, wheelRadius);
	}

	void RenderLoadingWheel(float cx, float cy, float radius) {
		float time = static_cast<float>(GetTime());
		float rotationOffset = time * 90;

		float wave = (sinf(time * 6.0f) + 1.0f) / 2.0f;

		float segmentArc = 60.0f;
		float thickness = 8.0f;

		for (int i = 0; i < 6; i++) {
			float startAngle = (i * segmentArc) + rotationOffset;
			float endAngle = startAngle + segmentArc - 10.0f;

			float alpha =
				(i % 2 == 0) ? (0.2f + wave * 0.8f) : (1.0f - wave * 0.8f);

			DrawRing({cx, cy}, radius - thickness, radius, startAngle, endAngle,
					 30, Fade(WHITE, alpha));
			DrawRing({cx, cy}, radius - thickness, radius, startAngle, endAngle,
					 30, Fade(WHITE, alpha * 0.5f));
		}
	}
};
