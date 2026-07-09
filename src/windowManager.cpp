#include "windowManager.hpp"
#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void WindowManager::Init() {
	InitWindow(screenWidth, screenHeight, windowTitle.c_str());
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	SetWindowState(FLAG_VSYNC_HINT);

	const int targetFPS = 60;
	SetTargetFPS(targetFPS);

	game.Init();
	menu.Init();

	currentState = WindowState::MENU;
}

void WindowManager::UpdateAndRenderFrame() {
	if (IsKeyPressed(KEY_F12)) {
		TakeScreenshot("screenshot.png");
	}

	Update();

	BeginDrawing();
	ClearBackground(GRAY);

	Render();

	EndDrawing();
}

#if defined(PLATFORM_WEB)
void WebFrameCallback(void *arg) {
	static_cast<WindowManager *>(arg)->UpdateAndRenderFrame();
}
#endif

void WindowManager::Run() {
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop_arg(WebFrameCallback, this, 0, 1);
#else
	while (!WindowShouldClose()) {
		UpdateAndRenderFrame();
	}
	CloseWindow();
#endif
}

void WindowManager::Update() {
	game.Update();
	switch (currentState) {
	case WindowState::GAME:
		game.Update();
		break;
	case WindowState::MENU:
		menu.Update(GetMousePosition());
		break;
	}
}

void WindowManager::Render() {
	game.Render();
	switch (currentState) {
	case WindowState::GAME:
		game.Render();
		break;
	case WindowState::MENU:
		menu.Render();
		break;
	}
}
