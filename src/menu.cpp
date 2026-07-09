#include "menu.hpp"
#include <raylib.h>
#include <string>

void Menu::Init() {
	InitButtons();
	UpdateButtonsPos();
}

void Menu::Reset() {
	shouldStartGame = false;
	shouldExit = false;
}

void Menu::Update(Vector2 mousePos) {
	animationTimer += GetFrameTime() * 0.5f;

	if (IsWindowResized()) {
		UpdateButtonsPos();
	}

	if (startGame.IsClicked(mousePos)) {
		shouldStartGame = true;
	}

	if (exit.IsClicked(mousePos)) {
		shouldExit = true;
	}

	startGame.Update(mousePos);
	exit.Update(mousePos);
}

void Menu::Render() {
	RenderBackground();
	RenderTitle();
	RenderVersion();

	startGame.Draw(GetFontDefault());
	exit.Draw(GetFontDefault());
}

void Menu::InitButtons() {
	startGame.text = "START";
	exit.text = "EXIT";
}

void Menu::UpdateButtonsPos() {
	int btnH{35};
	int btnSpacing{20};

	float btnX = 40.0f;

	auto exitY = static_cast<float>(GetScreenHeight() - btnH - 40);
	auto startY = static_cast<float>(exitY - btnH - btnSpacing);

	startGame.rect = {.x = btnX,
					  .y = startY,
					  .width = static_cast<float>(
						  MeasureText(startGame.text, startGame.fontSize)),
					  .height = static_cast<float>(startGame.fontSize)};

	exit.rect = {.x = btnX,
				 .y = exitY,
				 .width =
					 static_cast<float>(MeasureText(exit.text, exit.fontSize)),
				 .height = static_cast<float>(exit.fontSize)};
}

void Menu::RenderBackground() {
	float offsetY = sinf(animationTimer * 0.5f) * 40.0f;
	float offsetX = cosf(animationTimer * 0.3f) * 25.0f;

	float colorPulse = (sinf(animationTimer * 0.8f) + 1.0f) / 2.0f;
	auto dynamicRed = static_cast<unsigned char>(20 + (colorPulse * 60));

	Color topColor = {15, 15, 15, 255};
	Color bottomColor = {dynamicRed, 10, 10, 255};

	int paddingX = 50;
	int paddingY = 50;

	int drawX = -paddingX + offsetX;
	int drawY = -paddingY + offsetY;
	int drawW = GetScreenWidth() + (paddingX * 2);
	int drawH = GetScreenHeight() + paddingY * 2;

	DrawRectangleGradientV(drawX, drawY, drawW, drawH, topColor, bottomColor);
}

void Menu::RenderTitle() {
	int btnSpacing{40};

	std::string mainTitle = "DEADLINE";
	int titleSize{75};
	int titleX{btnSpacing};
	int titleY{100};

	DrawText(mainTitle.c_str(), titleX + 4, titleY + 4, titleSize,
			 Fade(BLACK, 0.8f));

	DrawText(mainTitle.c_str(), titleX, titleY, titleSize, WHITE);

	std::string subTile = "THE UNDEAD ONSLAUGHT";
	int subSize = 18;
	int subY = titleY + titleSize + 5;

	DrawText(subTile.c_str(), titleX + 2, subY + 2, subSize, Fade(BLACK, 0.5f));
	DrawText(subTile.c_str(), titleX, subY, subSize, MAROON);
}

void Menu::RenderVersion() {
	const char *versionText = "DEV BUILD (UNFINISHED)";

	int fontSize = 20;
	int margin = 40;

	int textWidth = MeasureText(versionText, fontSize);

	int posX = GetScreenWidth() - textWidth - margin;
	int posY = GetScreenHeight() - fontSize - margin;

	DrawText(versionText, posX + 1, posY + 1, fontSize, Fade(BLACK, 0.5f));
	DrawText(versionText, posX, posY, fontSize, GRAY);
}
