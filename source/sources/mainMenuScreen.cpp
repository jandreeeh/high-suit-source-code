#include "mainMenuScreen.h"

#include <iostream>
#include <raylib.h>

#include "button.h"
#include "screen.h"
#include "globals.h"
#include "game.h"

static Texture2D bg;
static Texture2D logo;
static Button playBtn("play", Vector2{ 32, 90 }, true);
static Button exitBtn("quit", Vector2{ 32, 120 }, true);
static Mouse mouse;
static float scale;


void MainMenuScreen::ScreenInit() {
	mouse.hitbox.width = 1;
	mouse.hitbox.height = 1;

	bg = LoadTexture("source/resources/sprites/bg/menu.png");
	logo = LoadTexture("source/resources/sprites/bg/logo.png");
	scale = MIN((float)GetScreenWidth() / VSCREEN_WIDTH, (float)GetScreenHeight() / VSCREEN_HEIGHT);
	playBtn.buttonInit();
	exitBtn.buttonInit();
}

void MainMenuScreen::ScreenUpdate(float dt){
	mouse.position.x = (GetMouseX() - (GetScreenWidth() - (VSCREEN_WIDTH * scale)) * 0.5f) / scale;
	mouse.position.y = (GetMouseY() - (GetScreenHeight() - (VSCREEN_HEIGHT * scale)) * 0.5f) / scale;
	mouse.hitbox.x = mouse.position.x;
	mouse.hitbox.y = mouse.position.y;
	playBtn.setVisible(true);
	exitBtn.setVisible(true);

	playBtn.buttonUpdate(dt, mouse);
	exitBtn.buttonUpdate(dt, mouse);
	if (playBtn.getSelected()) {
		GM.TransitionScreen(MAIN_GAME, 1);
	}
}
void MainMenuScreen::ScreenDraw(float dt){
	ClearBackground(RED);
	DrawTextEx(font, "Main Menu", Vector2{100, 100}, 16, 0, WHITE);

	DrawTexture(bg, 0, 0, WHITE);
	DrawTexture(logo, 20, 20, WHITE);

	playBtn.buttonDraw();
	exitBtn.buttonDraw();
}
int MainMenuScreen::getID() {
	return 20;
}
