#pragma once

#include <algorithm>
#include <raylib.h>

struct Blockade {
	int pos;
	float hp;
	float maxHp{100};

	void Heal(float val) {
		hp += val;
		hp = std::min(hp, maxHp);
	}
};
