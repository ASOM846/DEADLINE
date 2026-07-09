#pragma once
#include <raylib.h>
#include <raymath.h>

struct Button {
	Rectangle rect{0, 0, 0, 0};
	const char *text = "";
	float textOffsetX{0.0f};
	int fontSize{35};

	Button() = default;
	Button(Rectangle rectangle, const char *p_text, int p_fontSize = 35) {
		rect = rectangle;
		text = p_text;
		fontSize = p_fontSize;
		textOffsetX = 0.0f;
	}

	[[nodiscard]] bool IsClicked(const Vector2 mousePos) const {
		return (CheckCollisionPointRec(mousePos, rect) &&
				IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
	}

	[[nodiscard]] bool IsHovered(const Vector2 mousePos) const {
		return (CheckCollisionPointRec(mousePos, rect));
	}

	void Update(Vector2 mousePos) {
		float targetOffset = IsHovered(mousePos) ? 20.0f : 0.0f;

		textOffsetX = Lerp(textOffsetX, targetOffset, GetFrameTime() * 12.0f);
	}

	void Draw(const Font &font) const {
		DrawRectangleRec(rect, BLUE);

		bool hovered = (textOffsetX > 1.0f);
		Color textColor = hovered ? GOLD : WHITE;

		Vector2 textPos = {rect.x + textOffsetX, rect.y};
		Vector2 shadowPos = {rect.x + textOffsetX + 2.0f, rect.y + 2.0f};

		DrawTextEx(font, text, shadowPos, fontSize, 2.0f, Fade(BLACK, 0.6f));
		DrawTextEx(font, text, textPos, fontSize, 2.0f, textColor);
	}
};
