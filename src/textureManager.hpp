#pragma once

#include "weaponManager.hpp"
#include <raylib.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

enum class TextureId {
	PLAYER_PISTOL,
	PLAYER_DOUBLE_BARREL,
	PLAYER_RIFLE,
	PLAYER_SHOTGUN,
	PLAYER_SMG,
	PLAYER_SNIPER,

	ZOMBIE,

	GRASS,
	FLOOR,
	CRANE,
	BLOCKADE,

	PICKUP_HP,
	PICKUP_AMMO,

	WALL_STRAIGHT,
	WALL_SINGLE,

	WALL_END_BOTTOM,
	WALL_END_LEFT,
	WALL_END_RIGHT,
	WALL_END_TOP,

	WALL_JOIN_BOTTOM_TOP,
	WALL_JOIN_BOTTOM_LEFT,
	WALL_JOIN_BOTTOM_RIGHT,
	WALL_JOIN_TOP_RIGHT,
	WALL_JOIN_TOP_LEFT,

	WALL_JOIN_BOTTOM_RIGHT_TOP,
	WALL_JOIN_LEFT_TOP_RIGHT,
	WALL_JOIN_BOTTOM_LEFT_TOP,
	WALL_JOIN_BOTTOM_LEFT_RIGHT,
	WALL_ALL,

	NONE,
};

class TextureManager {
  public:
	TextureManager() = default;
	~TextureManager() { unloadAll(); };

	const Texture2D &get(TextureId id) const {
		auto it = textures.find(id);
		if (it == textures.end()) {
			throw std::runtime_error("No texture was found");
		}
		return it->second;
	}

	void loadAll() {
		load(TextureId::PLAYER_PISTOL, "assets/survivor_pistol.png");
		load(TextureId::PLAYER_DOUBLE_BARREL, "assets/survivor_shorty.png");
		load(TextureId::PLAYER_RIFLE, "assets/survivor_rifle.png");
		load(TextureId::PLAYER_SHOTGUN, "assets/survivor_shotgun.png");
		load(TextureId::PLAYER_SMG, "assets/survivor_smg.png");
		load(TextureId::PLAYER_SNIPER, "assets/survivor_sniper.png");
		load(TextureId::ZOMBIE, "assets/zombie.png");

		load(TextureId::GRASS, "assets/grass.png");
		load(TextureId::FLOOR, "assets/floor.png");
		load(TextureId::CRANE, "assets/crane.png");
		load(TextureId::BLOCKADE, "assets/blockade.png");

		load(TextureId::PICKUP_AMMO, "assets/ammo.png");
		load(TextureId::PICKUP_HP, "assets/hp.png");

		load(TextureId::WALL_STRAIGHT, "assets/walls/straight.png");
		load(TextureId::WALL_SINGLE, "assets/walls/single.png");
		load(TextureId::WALL_END_BOTTOM, "assets/walls/endDown.png");
		load(TextureId::WALL_END_LEFT, "assets/walls/endLeft.png");
		load(TextureId::WALL_END_RIGHT, "assets/walls/endRight.png");
		load(TextureId::WALL_END_TOP, "assets/walls/endTop.png");
		load(TextureId::WALL_JOIN_BOTTOM_TOP, "assets/walls/joinBottomTop.png");
		load(TextureId::WALL_JOIN_BOTTOM_LEFT, "assets/walls/joinLeft.png");
		load(TextureId::WALL_JOIN_BOTTOM_RIGHT, "assets/walls/joinRight.png");
		load(TextureId::WALL_JOIN_TOP_RIGHT, "assets/walls/joinTopRight.png");
		load(TextureId::WALL_JOIN_TOP_LEFT, "assets/walls/joinTopLeft.png");

		load(TextureId::WALL_JOIN_BOTTOM_RIGHT_TOP,
			 "assets/walls/joinBottomTopRight.png");
		load(TextureId::WALL_JOIN_LEFT_TOP_RIGHT,
			 "assets/walls/joinLeftTopRight.png");
		load(TextureId::WALL_JOIN_BOTTOM_LEFT_TOP,
			 "assets/walls/joinBottomLeftTop.png");
		load(TextureId::WALL_JOIN_BOTTOM_LEFT_RIGHT,
			 "assets/walls/joinLeftBottomRight.png");

		load(TextureId::WALL_ALL, "assets/walls/all.png");
	}

	void load(TextureId id, const char *path) {
		Texture2D tex = LoadTexture(path);
		if (tex.id == 0) {
			throw std::runtime_error(std::string("Unable to load: ") + path);
		}
		textures.emplace(id, tex);
	}

	void unloadAll() {
		for (auto &pair : textures) {
			UnloadTexture(pair.second);
		}
		textures.clear();
	}

	std::unordered_map<TextureId, Texture2D> textures;
};
