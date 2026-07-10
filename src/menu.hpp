#pragma once

#include "button.hpp"
#include <raylib.h>

class Menu {
  public:
	Menu() = default;
	~Menu() = default;

	void Init();
	void Reset();

	void Update(Vector2 mousePos);
	void Render();

	[[nodiscard]] bool ShouldStartGame() const { return shouldStartGame; }
	[[nodiscard]] bool ShouldSelectMap() const { return shouldSelectMap; }
	[[nodiscard]] bool ShouldExitGame() const { return shouldExit; }

	void RenderBackground();
	void RenderTitle();

  private:
	float animationTimer{0.0f};

	bool shouldStartGame{false};
	bool shouldSelectMap{false};
	bool shouldExit{false};

	int visualScreenWidth{};
	int visualScreenHeight{};

	Vector2 offset{0, 0};

	Button startGame;
	Button mapSelectionBtn;
	// Button settings;
	Button exit;

	void InitButtons();
	void UpdateButtonsPos();

	void RenderVersion();
};
