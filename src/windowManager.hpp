#pragma once
#include "game.hpp"
#include "menu.hpp"
#include "string"

enum class WindowState {
	GAME,
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
	void Update();
	void Render();

	Game game;
	Menu menu;

	WindowState currentState;

	int screenWidth{1280};
	int screenHeight{720};
	std::string windowTitle = "DEADLINE";
};
