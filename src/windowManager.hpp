#pragma once
#include "game.hpp"
#include "string"

class WindowManager {
  public:
	WindowManager() = default;
	~WindowManager() = default;

	void Init();

	void Run();

  private:
	void Update();
	void Render();

	Game game;

	int screenWidth{1280};
	int screenHeight{720};
	std::string windowTitle = "DEADLINE";
};
