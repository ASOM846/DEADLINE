#include "windowManager.hpp"
#include "gameStateManager.hpp"
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
	mapSelection.Init();

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
	ExitProgram();
#endif
}

void WindowManager::Update() {
	switch (currentState) {
	case WindowState::GAME:
		game.Update();
		break;
	case WindowState::LEVEL_SELECTION:
		mapSelection.Update();
		if (mapSelection.IsSelected()) {
			game.LoadMap(mapSelection.GetSelectedMap());
			SwitchState(WindowState::LOADING);
		}
		break;
	case WindowState::LOADING:
		loadingScreen.Update();
		if (loadingScreen.currentState == LoadingState::READY)
			SwitchState(WindowState::TRANSITION);
		break;
	case WindowState::TRANSITION:
		if (fadingToBlack) {
			transitionAlpha += GetFrameTime() * 2.0f;
			if (transitionAlpha >= 1.0f) {
				transitionAlpha = 1.0f;
				fadingToBlack = false;
				game.Reset();
				GetFrameTime();
			}
		} else {
			transitionAlpha -= GetFrameTime() * 2.0f;

			game.Update();

			if (transitionAlpha <= 0.0f) {
				transitionAlpha = 0.0f;
				currentState = WindowState::GAME;
			}
		}
		break;
	case WindowState::MENU:
		menu.Update(GetMousePosition());
		if (menu.ShouldStartGame()) {
			game.LoadMap(mapSelection.GetRandomMap());
			SwitchState(WindowState::LOADING);
		}

		if (menu.ShouldSelectMap())
			SwitchState(WindowState::LEVEL_SELECTION);

		if (menu.ShouldExitGame())
			ExitProgram();

		break;
	}
}

void WindowManager::Render() {
	switch (currentState) {
	case WindowState::GAME:
		game.Render();
		break;
	case WindowState::LEVEL_SELECTION:
		menu.RenderBackground();
		menu.RenderTitle();
		mapSelection.Render();
		break;
	case WindowState::LOADING:
		menu.RenderBackground();
		menu.RenderTitle();
		loadingScreen.Render();
		break;
	case WindowState::TRANSITION:
		if (fadingToBlack) {
			menu.RenderBackground();
			menu.RenderTitle();

			loadingScreen.Render();
		} else {
			game.Render();
		}

		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
					  Fade(BLACK, transitionAlpha));
		break;
	case WindowState::MENU:
		menu.Render();
		break;
	}
}

void WindowManager::SwitchState(WindowState newState) {
	if (newState == WindowState::MENU)
		menu.Reset();

	if (newState == WindowState::LOADING)
		loadingScreen.Reset();

	if (newState == WindowState::TRANSITION) {
		transitionAlpha = 0.0f;
		fadingToBlack = true;
	}
	currentState = newState;
}

void WindowManager::ExitProgram() {
	CloseWindow();
}
