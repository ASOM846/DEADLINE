#pragma once

#include "textureManager.hpp"
#include <cstdlib>
#include <raylib.h>
#include <stdexcept>
#include <unordered_map>

enum class SoundId {
	GUNFIRE,
	RELOAD,
};

enum class MusicId {
	BCG_MUSIC,
};

class AudioManager {
  public:
	AudioManager() = default;
	~AudioManager() {
		for (auto &pair : sounds)
			UnloadSound(pair.second);
		for (auto &pair : musics)
			UnloadMusicStream(pair.second);
	}

	void PlaySound2(SoundId id) { PlaySound(get(id)); }

	void PlaySoundRandomozed(SoundId id, float minPitch = 0.9,
							 float maxPitch = 1.1f) {
		Sound &s = get(id);
		auto randomPich = static_cast<float>(
			GetRandomValue(minPitch * 100, maxPitch * 100) / 100.0F);

		SetSoundPitch(s, randomPich);
		PlaySound(s);
	}

	void PlayMusic(MusicId id) { PlayMusicStream(musics.at(id)); }

	void ResetMusic(MusicId id) {
		Music &m = musics.at(id);
		StopMusicStream(m);
		PlayMusicStream(m);
	}

	Sound &get(SoundId id) {
		auto it = sounds.find(id);
		if (it == sounds.end()) {
			throw std::runtime_error("No sound was found");
		}
		return it->second;
	}

	void loadAll() {
		loadSounds();
		loadMusics();
	}

	void loadSounds() {
		load(SoundId::GUNFIRE, "assets/gun1.mp3");
		load(SoundId::RELOAD, "assets/reload.mp3");
	}

	void loadMusics() { load(MusicId::BCG_MUSIC, "assets/music.mp3"); }

	void load(SoundId id, const char *path) {
		Sound sound = LoadSound(path);
		sounds.emplace(id, sound);
	}

	void load(MusicId id, const char *path) {
		Music music = LoadMusicStream(path);
		musics.emplace(id, music);
	}

	void UpdateMusic(MusicId id) {
		UpdateMusicStream(musics.at(id));
		if (!IsMusicStreamPlaying(musics.at(id))) {
			PlayMusicStream(musics.at(id));
		}
	}

	std::unordered_map<SoundId, Sound> sounds;
	std::unordered_map<MusicId, Music> musics;
};
