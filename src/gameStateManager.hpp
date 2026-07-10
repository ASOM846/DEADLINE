#pragma once

#include "entity/player.hpp"

enum class GameState {
	PLAYING,
	PAUSED,
	LOST,
	RETURN_TO_MENU,
};

class GameStateManager {
  private:
	GameState currentState{};

  public:
	void Init() { currentState = GameState::PLAYING; }

	void Update(const Player &player) {
		if (player.hp <= 0 && currentState == GameState::PLAYING) {
			currentState = GameState::LOST;
		}
	}

	[[nodiscard]] GameState GetCurrentState() const { return currentState; }

	void SwitchState(GameState newState) {
		if (newState == currentState) {
			return;
		}

		currentState = newState;
	}
};
