#include "gameScreen.h"

#include <iostream>
#include <raylib.h>

#include "screen.h"
#include "globals.h"
#include "game.h"
#include "deck.h"
#include "button.h"

static enum stages {
	BEGIN = 90,
	DECK_BUILD = 91,
	BOSS_BATTLE = 92,
};

static Deck mainDeck(false);
static Deck playerDeck(true);
static Deck bossDeck(true);
static Deck selectDeck(true);
static Mouse mouse;
static Button nextBtn("next", Vector2{262, 125}, true);

static int totaldeck = 52;
static int setup = true;
static float scale;
static bool jokerIn = true;

void setRandomCards();

void GameScreen::ScreenInit() {
	if (setup) {
		setup = BEGIN;
		FormGeneralDeck(mainDeck);
		int index;

		for (int i = 0; i < 5; i++) {
			index = GetRandomValue(0, 8);
			if (index == 2 && jokerIn) {
				bossDeck.insertJoker();
				jokerIn = false;
			}
			else {
				//i--;
				index = GetRandomValue(0, totaldeck);
				std::cout << index << "\n";
				bossDeck.insertExisitingCard(mainDeck.transferCard(index));
				totaldeck--;
			}
		}
		nextBtn.buttonInit();
		setup = false;
	}

	//bossDeck.displayDeck();

	scale = MIN((float)GetScreenWidth() / VSCREEN_WIDTH, (float)GetScreenHeight() / VSCREEN_HEIGHT);
	mouse.hitbox.width = 1;
	mouse.hitbox.height = 1;
}

void GameScreen::ScreenUpdate(float dt) {
	//Screen mouse position to virtual screen position
	mouse.position.x = (GetMouseX() - (GetScreenWidth() - (VSCREEN_WIDTH * scale)) * 0.5f) / scale;
	mouse.position.y = (GetMouseY() - (GetScreenHeight() - (VSCREEN_HEIGHT * scale)) * 0.5f) / scale;
	mouse.hitbox.x = mouse.position.x;
	mouse.hitbox.y = mouse.position.y;

	nextBtn.buttonUpdate(dt, mouse);
	bossDeck.deckUpdate(dt, mouse);
}
void GameScreen::ScreenDraw(float dt) {
	ClearBackground(GREEN);
	DrawTextEx(font, "Main Game", Vector2{ 100, 100 }, 16, 0, WHITE);
	bossDeck.deckDraw();
	nextBtn.buttonDraw();
}

int GameScreen::getID() { 
	return 10; 
}

void setRandomCards() {
	for (int i = 0; i < 3; i++) {
		int index = GetRandomValue(0, totaldeck);
		std::cout << index << "\n";
		selectDeck.insertExisitingCard(mainDeck.transferCard(index));
		totaldeck--;
	}
}