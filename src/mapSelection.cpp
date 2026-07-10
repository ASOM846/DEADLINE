#include "mapSelection.hpp"
#include "levelMap.hpp"
#include <raylib.h>

void MapSelection::Init() {
	LevelMap map1;
	map1.width = 75;
	map1.height = 75;
	map1.cellSize = 40;
	map1.filename = "assets/map1.txt";
	map1.title = "MAP 1 \n SMALL \n (FOR WEB BUILD)";
	maps.push_back(map1);

	LevelMap map2;
	map2.width = 150;
	map2.height = 150;
	map2.cellSize = 40;
	map2.filename = "assets/map2.txt";
	map2.title = "MAP 2 \n BIG \n (FOR \n DESKTOP BUILD)";
	maps.push_back(map2);

	LevelMap map3;
	map3.width = 150;
	map3.height = 150;
	map3.cellSize = 40;
	map3.filename = "assets/map3.txt";
	map3.title = "MAP 3 \n BIG \n (FOR  \n DESKTOP BUILD)";
	maps.push_back(map3);

	goBackBtn.text = "BACK";
}

void MapSelection::Reset() {
	isSelected = false;
	goBack = false;
}

void MapSelection::Update() {
	Vector2 mousePos = GetMousePosition();
	hoverIndex = -1;

	int btnW = MeasureText(goBackBtn.text, goBackBtn.fontSize);
	goBackBtn.rect = {.x = static_cast<float>((GetScreenWidth() - btnW) / 2),
					  .y = static_cast<float>(GetScreenHeight() * 4 / 5),
					  .width = static_cast<float>(btnW),
					  .height = static_cast<float>(goBackBtn.fontSize)};

	goBackBtn.Update(mousePos);

	if (goBackBtn.IsClicked(mousePos)) {
		goBack = true;
	}

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

	goBackBtn.Draw(GetFontDefault());

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
