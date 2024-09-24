#include <raylib.h>

#include "minigameScreen.h"
#include "minigame.h"
#include "game.h"

Minigame* selectedMinigame;

void MinigameScreen::ScreenInit() {

}
void MinigameScreen::ScreenUpdate(float dt) {
	ClearBackground(BLUE);
	DrawTextEx(font, "Minigame", Vector2{ 100, 100 }, 16, 0, WHITE);
}
void MinigameScreen::ScreenDraw(float dt) {

}