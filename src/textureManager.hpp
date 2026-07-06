#pragma once

#include <raylib.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

enum class TextureId {
	PLAYER_PISTOL,
	PLAYER_RIFLE,
	PLAYER_SHOTGUN,
	PLAYER_SMG,

	ZOMBIE,
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
		load(TextureId::PLAYER_RIFLE, "assets/survivor_rifle.png");
		load(TextureId::PLAYER_SHOTGUN, "assets/survivor_shotgun.png");
		load(TextureId::PLAYER_SMG, "assets/survivor_smg.png");
		load(TextureId::ZOMBIE, "assets/zombie.png");
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
