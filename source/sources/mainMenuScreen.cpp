#include "mainMenuScreen.h"

#include <iostream>
#include <raylib.h>

#include "screen.h"
#include "globals.h"
#include "game.h"

void MainMenuScreen::ScreenUpdate(float dt){
	if (IsKeyPressed(KEY_A)) {
		GM.TransitionScreen(MAIN_GAME, 0.5);
	}
}
void MainMenuScreen::ScreenDraw(float dt){
	ClearBackground(RED);
	DrawText("Main Menu", 100, 100, 10, WHITE);
}
int MainMenuScreen::getID() {
	return 20;
}
