#pragma once

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

	void Reset() { loadingTimer = 0.0f; }

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
		DrawText("LOADING", GetScreenWidth() / 2, GetScreenHeight() / 2, 70,
				 RED);
	}

	// void RenderLoadingWheel(float cx, float cy, float radius) {
	// 	float wave = (sing(anima))
	// }
};
