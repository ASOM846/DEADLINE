#include "mapSelection.hpp"
#include <raylib.h>

void MapSelection::Init() {
	LevelMap map1;
	map1.width = 100;
	map1.height = 100;
	map1.cellSize = 40;
	map1.filename = "assets/map.txt";
	map1.title = "MAP_1";

	maps.push_back(map1);
	maps.push_back(map1);
	maps.push_back(map1);
}

void MapSelection::Update() {
	Vector2 mousePos = GetMousePosition();
	hoverIndex = -1;

	int countX = static_cast<int>(maps.size());
	if (countX == 0)
		return;

	int spacing = (GetScreenWidth() - (countX * width)) / (countX + 1);
	int startY = (GetScreenHeight() - height) / 2;

	for (int i = 0; i < countX; i++) {
		float startX = static_cast<float>(spacing + i * (width + spacing));

		Rectangle cardRect = {startX, static_cast<float>(startY),
							  static_cast<float>(width),
							  static_cast<float>(height)};

		if (CheckCollisionPointRec(mousePos, cardRect)) {
			hoverIndex = i;

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				isSelected = true;
			}
		}
	}
}

void MapSelection::Render() {
	int countX = static_cast<int>(maps.size());
	if (countX == 0)
		return;

	int spacing = (GetScreenWidth() - (countX * width)) / (countX + 1);
	int startY = (GetScreenHeight() - height) / 2;

	for (int i = 0; i < countX; i++) {
		float cardX = spacing + (i * (spacing + width));
		float cardY = startY;

		bool isHovered = (hoverIndex == i);

		if (isHovered)
			cardY -= 10.0f;

		Rectangle cardRect = {cardX, cardY, static_cast<float>(width),
							  static_cast<float>(height)};

		DrawRectangleRounded({cardX + 4, cardY + 6, static_cast<float>(width),
							  static_cast<float>(height)},
							 0.05F, 4, Fade(BLACK, 0.4f));

		Color cardColor =
			isHovered ? Color{40, 40, 45, 255} : Color{25, 25, 28, 255};
		DrawRectangleRounded(cardRect, 0.05f, 4, cardColor);

		Color borderColor = isHovered ? RED : Color{80, 80, 85, 255};
		DrawRectangleRoundedLines(cardRect, 0.05f, 4, borderColor);

		std::string title = maps[i].title;
		int fontSize = 20;
		int textWidth = MeasureText(title.c_str(), fontSize);
		int textX = cardX + (width - textWidth) / 2;

		DrawText(title.c_str(), textX, cardY + 30, fontSize,
				 isHovered ? WHITE : LIGHTGRAY);

		std::string info = std::to_string(maps[i].width) + " x " +
						   std::to_string(maps[i].height);

		int infoSize = 16;
		int infoWidth = MeasureText(info.c_str(), infoSize);
		int infoX = static_cast<int>(cardX + (width - infoWidth) / 2);

		DrawText(info.c_str(), infoX, cardY + height - 40, infoSize, MAROON);
	}
}
