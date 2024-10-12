#include <raylib.h>

#include "minigameScreen.h"
#include "minigame.h"
#include "minigames/minigameTest.h"
#include "minigames/minigameArrow.h"
#include "game.h"


//MinigameTest minigameTest;
MinigameArrow minigameArrow;
Minigame* currentMinigame;

static bool isStart = false;



void MinigameScreen::ScreenInit() {
	MinigameHandler.setMinigameDone(false);

	currentMinigame = &minigameArrow;
	//currentMinigame = &minigameTest;

	currentMinigame->MinigameInit();
	isStart = false;
}
void MinigameScreen::ScreenUpdate(float dt) {
	if (!isStart) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			isStart = true;
		}
	}
	else {
		currentMinigame->MinigameUpdate();
	}

	if (MinigameHandler.getMinigameDone()) {
		isStart = false;
		GM.TransitionScreen(MAIN_GAME, 0.5f);
	}
}
void MinigameScreen::ScreenDraw(float dt) {
	ClearBackground(BLUE);
	//DrawTextEx(font, "Minigame", Vector2{ 100, 100 }, 16, 0, WHITE);
	currentMinigame->MinigameDraw();
	if (!isStart && !MinigameHandler.getMinigameDone()) {
		DrawRectangle(0, 0, VSCREEN_WIDTH, VSCREEN_HEIGHT, Fade(BLACK, 0.5f));
		DrawTextEx(font, "PRESS Left Mouse Button TO START", Vector2{ ((float)VSCREEN_WIDTH / 2) - (MeasureTextEx(font,"PRESS Left Mouse Button TO START", 16, 0).x / 2), VSCREEN_HEIGHT / 2 }, 16, 0, WHITE);


	}

}

void MinigameScreen::ScreenDeInit() {
	currentMinigame->MinigameDeInit();
}