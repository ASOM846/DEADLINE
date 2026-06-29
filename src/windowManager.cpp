#include "windowManager.hpp"
#include "raylib.h"

void WindowManager::Init() {
	InitWindow(screenWidth, screenHeight, windowTitle.c_str());

	const int targetFPS = 60;
	SetTargetFPS(targetFPS);
}

void WindowManager::Run() {
	while (!WindowShouldClose()) {
		Update();

		BeginDrawing();
		ClearBackground(GRAY);

		Render();

		EndDrawing();
	}
}

void WindowManager::Update() {
	game.Update();
}

void WindowManager::Render() {
	game.Render();
}
