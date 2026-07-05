#pragma once

enum class PotionType {
	SPEED,
	THIRD_SLOT,
	RAPID_FIRE,
	HEAL
};

struct PotionSpawner {
	int pos{};
	PotionType type{};
};
