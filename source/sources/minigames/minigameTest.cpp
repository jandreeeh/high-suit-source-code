#include "minigames/minigameTest.h"

#include <raylib.h>

#include "globals.h"
#include "game.h"

static bool gameDone = false;

void MinigameTest::MinigameInit() {
	gameDone = false;
}
void MinigameTest::MinigameUpdate() {
	if (!gameDone) {
		if (IsKeyPressed(KEY_Y)) {
			gameDone = true;
			MinigameHandler.setMinigameStats(true);
		}
		if (IsKeyPressed(KEY_N)) {
			gameDone = true;
			MinigameHandler.setMinigameStats(false);
		}
	}
	else {
		if (IsKeyPressed(KEY_ENTER)) {
			MinigameHandler.setMinigameDone(true);
		}
	}
}
void MinigameTest::MinigameDraw() {
	DrawTextEx(font, "Nah you [Y/N]", Vector2{ 100, 100 }, 16, 0, WHITE);
	if (gameDone) {
		DrawTextEx(font, "Enter to exit", Vector2{ 80, 80 }, 16, 0, WHITE);

	}
}
void MinigameTest::MinigameDeInit() {

}