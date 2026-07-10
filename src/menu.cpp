#include "menu.hpp"
#include <raylib.h>
#include <string>

void Menu::Init() {
	InitButtons();
	UpdateButtonsPos();
}

void Menu::Reset() {
	shouldStartGame = false;
	shouldSelectMap = false;
	shouldExit = false;
}

void Menu::Update(Vector2 mousePos) {
	animationTimer += GetFrameTime() * 0.5f;

	visualScreenWidth = (GetScreenWidth() > 1600) ? 1600 : GetScreenWidth();
	visualScreenHeight = (GetScreenHeight() > 900) ? 900 : GetScreenHeight();

	offset.x = (GetScreenWidth() - visualScreenWidth) / 2.0f;
	offset.y = (GetScreenHeight() - visualScreenHeight) / 2.0f;

	if (IsWindowResized()) {
		UpdateButtonsPos();
	}

	if (startGame.IsClicked(mousePos)) {
		shouldStartGame = true;
	}

	if (mapSelectionBtn.IsClicked(mousePos)) {
		shouldSelectMap = true;
	}

	if (exit.IsClicked(mousePos)) {
		shouldExit = true;
	}

	startGame.Update(mousePos);
	mapSelectionBtn.Update(mousePos);
	settings.Update(mousePos);
	exit.Update(mousePos);
}

void Menu::Render() {
	RenderBackground();
	RenderTitle();
	RenderVersion();

	startGame.Draw(GetFontDefault());
	mapSelectionBtn.Draw(GetFontDefault());
	settings.Draw(GetFontDefault());
	exit.Draw(GetFontDefault());
}

void Menu::InitButtons() {
	startGame.text = "QUICK START";
	mapSelectionBtn.text = "MAPS";
	settings.text = "SETTINGS";
	exit.text = "EXIT";
}

void Menu::UpdateButtonsPos() {
	int btnH{35};
	int btnSpacing{30};

	float btnX = 40.0f + offset.x;

	auto exitY = static_cast<float>(GetScreenHeight() - offset.y - btnH - 40);
	auto settingsY = static_cast<float>(exitY - btnH - btnSpacing);
	auto mapSelectionY = static_cast<float>(settingsY - btnH - btnSpacing);
	auto startY = static_cast<float>(mapSelectionY - btnH - btnSpacing);

	startGame.rect = {.x = btnX,
					  .y = startY,
					  .width = static_cast<float>(
						  MeasureText(startGame.text, startGame.fontSize)),
					  .height = static_cast<float>(startGame.fontSize)};

	mapSelectionBtn.rect = {
		.x = btnX,
		.y = mapSelectionY,
		.width = static_cast<float>(
			MeasureText(mapSelectionBtn.text, mapSelectionBtn.fontSize)),
		.height = static_cast<float>(mapSelectionBtn.fontSize)};

	settings.rect = {.x = btnX,
					 .y = settingsY,
					 .width = static_cast<float>(
						 MeasureText(settings.text, settings.fontSize)),
					 .height = static_cast<float>(settings.fontSize)};

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
	int titleX{static_cast<int>(btnSpacing + offset.x)};
	int titleY{static_cast<int>(offset.y + 30)};

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

	int posX = GetScreenWidth() - textWidth - margin - offset.x;
	int posY = GetScreenHeight() - fontSize - margin - offset.y;

	DrawText(versionText, posX + 1, posY + 1, fontSize, Fade(BLACK, 0.5f));
	DrawText(versionText, posX, posY, fontSize, GRAY);
}
