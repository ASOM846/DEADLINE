#include "windowManager.hpp"
#include "raylib.h"

void WindowManager::Init() {
	InitWindow(screenWidth, screenHeight, windowTitle.c_str());
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	const int targetFPS = 60;
	SetTargetFPS(targetFPS);

	game.Init();
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
