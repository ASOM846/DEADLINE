#include "player.hpp"
#include <raylib.h>

void Player::Init() {
	weaponManager.Init();
}

void Player::Update(std::vector<Bullet> &bullets, Vector2 worldMousePos,
					LevelMap &map) {
	HandleMovement();

	HandleWeaponActions();
	HandleDoorActions(map);
	HandlePickableActions();
	HandleBlockadeActions();

	weaponManager.Update(bullets, position, worldMousePos);
}

void Player::Render() const {
	DrawCircleV(position, radius, BLUE);
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

void Player::HandleWeaponActions() {
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
