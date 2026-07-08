#include "player.hpp"
#include "../types.hpp"
#include "bullet.hpp"
#include <raylib.h>

void Player::Init() {
	weaponManager.Init();

	Reset();
}

void Player::Reset() {
	hp = maxHp;
	money = 1000000;

	hasPotionSpeed = false;
	hasPotionThirdSlot = false;
	hasPotionRapidFire = false;
	hasPotionHeal = false;
}

void Player::Update(std::vector<Bullet> &bullets, Vector2 worldMousePos,
					LevelMap &map, ScreenShake &screenShake,
					EffectManager &effectManager) {
	float angleRad =
		std::atan2(worldMousePos.y - position.y, worldMousePos.x - position.x);

	rotation = angleRad * 57.29578F;

	HandleMovement();

	HandleKnifeActions(worldMousePos);
	HandleWeaponActions(bullets);
	HandleRandomSpawnerActions();
	HandlePotionSpawnActions();
	HandleDoorActions(map);
	HandlePickableActions();
	HandleBlockadeActions();

	weaponManager.Update(bullets, position, worldMousePos, screenShake,
						 hasPotionRapidFire, angleRad);
}

void Player::Render(TextureManager &tm) {
	// DrawCircleV(position, radius, BLUE);

	Weapon temp = *weaponManager.GetCurrentWeapon();
	WeaponType type = temp.type;

	TextureId playerTexId = TextureId::PLAYER_PISTOL;

	switch (type) {
	case WeaponType::PISTOL:
		playerTexId = TextureId::PLAYER_PISTOL;
		break;
	case WeaponType::DOUBLE_BARREL:
		playerTexId = TextureId::PLAYER_DOUBLE_BARREL;
		break;
	case WeaponType::UZI:
		playerTexId = TextureId::PLAYER_SMG;
		break;
	case WeaponType::AK47:
		playerTexId = TextureId::PLAYER_RIFLE;
		break;
	case WeaponType::SNIPER:
		playerTexId = TextureId::PLAYER_SNIPER;
		break;
	case WeaponType::SHOTGUN:
		playerTexId = TextureId::PLAYER_SHOTGUN;
		break;
	case WeaponType::COUNT:
		break;
	}

	float localOffsetY = 10.0f;

	float textureScale = 1.3f;

	float drawWidth = visualRadius * 2.0f * textureScale;
	float drawHeight = visualRadius * 2.0f * textureScale;

	Texture2D playerTex = tm.get(playerTexId);

	Rectangle src = {0, 0, static_cast<float>(playerTex.width),
					 static_cast<float>(playerTex.height)};

	Rectangle dstCorrected = {position.x, position.y, drawWidth, drawHeight};

	Vector2 origin = {drawWidth / 2.0f, (drawHeight / 2.0f) + localOffsetY};

	DrawTexturePro(playerTex, src, dstCorrected, origin,
				   rotation + textureRotation, RAYWHITE);

	if (knifeVisualTimer > 0.0f) {
		float angle = std::atan2(knifeDir.y, knifeDir.x) * 57.29578f;

		DrawCircleSector(position, knifeRange, angle - 60.0f, angle + 60.0f, 16,
						 Fade(LIGHTGRAY, 0.6f));
	}
}

void Player::HandleMovement() {
	velocity = {.x = 0, .y = 0};

	int buffSpeed = speed;
	if (hasPotionSpeed)
		buffSpeed += 1.5f;

	if (IsKeyDown(KEY_W)) {
		velocity.y -= buffSpeed;
	}

	if (IsKeyDown(KEY_S)) {
		velocity.y += buffSpeed;
	}

	if (IsKeyDown(KEY_A)) {
		velocity.x -= buffSpeed;
	}

	if (IsKeyDown(KEY_D)) {
		velocity.x += buffSpeed;
	}
}

void Player::HandleKnifeActions(Vector2 worldMousePos) {
	if (knifeCooldownTimer > 0.0f)
		knifeCooldownTimer -= GetFrameTime();
	if (knifeVisualTimer > 0.0f)
		knifeVisualTimer -= GetFrameTime();

	if (IsKeyPressed(KEY_V) && knifeCooldownTimer <= 0.0f) {
		knifeTriggered = true;
		knifeCooldownTimer = knifeCooldownDuration;
		knifeVisualTimer = 0.1;

		knifeDir.x = worldMousePos.x - position.x;
		knifeDir.y = worldMousePos.y - position.y;

		float dist =
			std::sqrt(knifeDir.x * knifeDir.x + knifeDir.y * knifeDir.y);

		if (dist > 0.0f) {
			knifeDir.x /= dist;
			knifeDir.y /= dist;
		} else {
			knifeDir = {1.0, 0.0f};
		}
	}
}

void Player::HandleWeaponActions(std::vector<Bullet> &bullets) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		weaponManager.SwitchWeaponNext();
	}

	if (IsKeyDown(KEY_R)) {
		weaponManager.StartReload();
	}

	if (!IsKeyPressed(KEY_E) || currentSpawner == nullptr) {
		return;
	}

	Weapon *w = weaponManager.GetCurrentWeapon();

	if (w == nullptr) {
		return;
	}

	if (currentSpawner->type == w->type && money >= w->magazinePrice &&
		w->ammo < w->maxAmmo) {
		w->ammo += w->magazinePrice;

		w->ammo = std::min(w->ammo, w->maxAmmo);

		money -= w->magazinePrice;

	} else if (currentSpawner->type != w->type) {
		Weapon *templateWep =
			weaponManager.GetWeaponTemplate(currentSpawner->type);

		if (templateWep != nullptr && money >= templateWep->price) {
			weaponManager.GiveWeapon(currentSpawner->type);
			money -= templateWep->price;
		}
	}
}

void Player::HandleRandomSpawnerActions() {
	if (currentRandomSpawner == nullptr) {
		return;
	}

	RandomWeaponSpawner *ws = currentRandomSpawner;

	switch (ws->state) {
	case RandomWeaponSpawnerState::IDLE:
		if (IsKeyPressed(KEY_E) && CanBuy(ws->price))
			ws->state = RandomWeaponSpawnerState::ROLLING;
		break;
	case RandomWeaponSpawnerState::ROLLING:
		break;
	case RandomWeaponSpawnerState::DRAWN:
		if (IsKeyPressed(KEY_E)) {
			weaponManager.GiveWeapon(ws->drawnType);
			ws->Decline();
		}
		if (IsKeyPressed(KEY_SPACE))
			ws->Decline();
		break;
	}
}

void Player::HandleDoorActions(LevelMap &map) {
	if (IsKeyPressed(KEY_E) && currentDoor != nullptr &&
		money >= currentDoor->price) {
		int doorTileIndex = currentDoor->pos;
		money -= currentDoor->price;

		int upperIdx{doorTileIndex - map.width};
		int lowerIdx{doorTileIndex + map.width};
		int leftIdx{doorTileIndex - 1};
		int rightIdx{doorTileIndex + 1};

		int lastNeighbour{};

		for (auto &d : map.doors) {
			if (d.pos == upperIdx) {
				lastNeighbour = upperIdx;
				break;
			}

			if (d.pos == lowerIdx) {
				lastNeighbour = lowerIdx;
				break;
			}

			if (d.pos == leftIdx) {
				lastNeighbour = leftIdx;
				break;
			}

			if (d.pos == rightIdx) {
				lastNeighbour = rightIdx;
				break;
			}
		}

		map.tiles[lastNeighbour] = TileType::FLOOR;

		map.doors.erase(std::remove_if(map.doors.begin(), map.doors.end(),
									   [lastNeighbour](const Door &d) {
										   return d.pos == lastNeighbour;
									   }),
						map.doors.end());

		map.tiles[doorTileIndex] = TileType::FLOOR;

		map.doors.erase(std::remove_if(map.doors.begin(), map.doors.end(),
									   [doorTileIndex](const Door &d) {
										   return d.pos == doorTileIndex;
									   }),
						map.doors.end());

		currentDoor = nullptr;
	}
}

void Player::HandlePickableActions() {
	if (currentPickable == nullptr)
		return;

	switch (currentPickable->type) {
	case PickableType::HP:
		hp = 100;
		break;
	case PickableType::AMMO:
		Weapon *w = weaponManager.GetCurrentWeapon();
		w->ammo = w->maxAmmo;
		break;
	}
}

void Player::HandleBlockadeActions() {
	if (currentBlockade != nullptr && IsKeyDown(KEY_E)) {
		currentBlockade->Heal(10 * GetFrameTime());
	}
}

bool Player::CanBuy(int val) {
	if (money >= val) {
		money -= val;
		return true;
	}
	return false;
}

void Player::HandlePotionSpawnActions() {
	if (currentPotionSpawn == nullptr)
		return;

	if (!IsKeyPressed(KEY_E)) {
		return;
	}

	switch (currentPotionSpawn->type) {
	case PotionType::SPEED:
		if (hasPotionSpeed)
			return;
		hasPotionSpeed = true;
		break;
	case PotionType::THIRD_SLOT:
		if (hasPotionThirdSlot)
			return;
		weaponManager.maxSlots = 3;
		hasPotionThirdSlot = true;
		break;
	case PotionType::RAPID_FIRE:
		if (hasPotionRapidFire)
			return;
		hasPotionRapidFire = true;
		break;
	case PotionType::HEAL:
		if (hasPotionHeal)
			return;
		hasPotionHeal = true;
		break;
	}
}
