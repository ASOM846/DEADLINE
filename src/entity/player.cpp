#include "player.hpp"
#include "bullet.hpp"
#include <raylib.h>

void Player::Init() {
	weaponManager.Init();
}

void Player::Update(std::vector<Bullet> &bullets, Vector2 worldMousePos,
					LevelMap &map, ScreenShake &screenShake) {
	HandleMovement();

	HandleKnifeActions(worldMousePos);
	HandleWeaponActions(bullets);
	HandleRandomSpawnerActions();
	HandlePotionSpawnActions();
	HandleDoorActions(map);
	HandlePickableActions();
	HandleBlockadeActions();

	weaponManager.Update(bullets, position, worldMousePos, screenShake,
						 hasPotionRapidFire);
}

void Player::Render() const {
	DrawCircleV(position, radius, BLUE);

	if (knifeVisualTimer > 0.0f) {
		float angle = std::atan2(knifeDir.y, knifeDir.x) * 57.29578f;

		DrawCircleSector(position, knifeRange, angle - 60.0f, angle + 60.0f, 16,
						 Fade(LIGHTGRAY, 0.6f));
	}
}

void Player::HandleMovement() {
	velocity = {.x = 0, .y = 0};

	if (IsKeyDown(KEY_W)) {
		velocity.y -= speed;
	}

	if (IsKeyDown(KEY_S)) {
		velocity.y += speed;
	}

	if (IsKeyDown(KEY_A)) {
		velocity.x -= speed;
	}

	if (IsKeyDown(KEY_D)) {
		velocity.x += speed;
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

	} else if (currentSpawner->type != w->type &&
			   money >= spawnerWeapon->price) {
		weaponManager.SwitchWeaponTo(currentSpawner->type);
		money -= spawnerWeapon->price;
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
			weaponManager.SwitchWeaponTo(ws->drawnType);
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

	TraceLog(LOG_INFO, "COLLISION");
	switch (currentPotionSpawn->type) {
	case PotionType::SPEED:
		if (hasPotionSpeed)
			return;
		hasPotionSpeed = true;
		break;
	case PotionType::THIRD_SLOT:
		if (hasPotionThirdSlot)
			return;
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
