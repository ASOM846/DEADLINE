#pragma once
#include "game.hpp"
#include "loadingScreen.hpp"
#include "menu.hpp"
#include "string"

enum class WindowState {
	GAME,
	LOADING,
	TRANSITION,
	MENU,
};

class WindowManager {
  public:
	WindowManager() = default;
	~WindowManager() = default;

	void Init();

	void Run();

	void UpdateAndRenderFrame();

  private:
	float transitionAlpha = 0.0F;
	bool fadingToBlack = true;

	void Update();
	void Render();

	void SwitchState(WindowState newState);

	void ExitProgram();

	Game game;
	Menu menu;
	LoadingScreen loadingScreen;

	WindowState currentState;

	int screenWidth{1280};
	int screenHeight{720};
	std::string windowTitle = "DEADLINE";
};
