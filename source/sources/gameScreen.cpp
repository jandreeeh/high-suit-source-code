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
static enum playerState {
	CUTSCENE,
	INPUT
};

static Deck mainDeck(false);
static Deck bossDeck(false);
static Deck playerDeck(false);
static Deck selectDeck(false);
static Mouse mouse;
static Button nextBtn("next", Vector2{ 262, 125 }, true);
static Button selectBtn("select", Vector2{250, 115}, true);
static Texture2D bg;

static int totaldeck = 52;
static bool setup = true;
static bool deckSetup = true;
static bool jokerIn = true;
static int deckBuildStage = 0;
static float scale;

static float gameClock;

static stages currentStage;
static playerState pState;

void setRandomCards();

void GameScreen::ScreenInit() {
	nextBtn.setVisible(false);
	selectBtn.setVisible(false);
	deckSetup = true;
	bg = LoadTexture("source/resources/sprites/bg/table.png");

	if (currentStage == DECK_BUILD && deckSetup) {
		setRandomCards();
	}

	if (setup) {
		currentStage = BEGIN;
		pState = CUTSCENE;
		gameClock = 0;

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
		selectBtn.buttonInit();
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
	selectBtn.buttonUpdate(dt, mouse);
	bossDeck.deckUpdate(dt, mouse);
	selectDeck.deckUpdate(dt, mouse);

	switch (currentStage) {
	case BEGIN:
		if (pState == CUTSCENE) {
			gameClock += dt;
			if (gameClock > 0.5) { bossDeck.setSliding(true); }

			if (bossDeck.getSliding()) {
				bossDeck.slideCards(Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), -40 }, Vector2{ 70, 15 }, dt, true, 0.5);
			}
			if (bossDeck.getDoneSliding()) {
				pState = INPUT;
				nextBtn.setVisible(true);
			}
		}
		else if (pState == INPUT) {
			if (nextBtn.getSelected()) {
				pState = CUTSCENE;
				currentStage = DECK_BUILD;
				GM.TransitionScreen(MINIGAME, 0.5);
			}
		}
		break;
	case DECK_BUILD:
		if (pState == CUTSCENE) {
			gameClock += dt;
			if (gameClock > 2) { selectDeck.setSliding(true); }

			if (selectDeck.getSliding()) {
				selectDeck.slideCards(Vector2{-100, 100}, Vector2{ 100, VSCREEN_HEIGHT/2 }, dt, true, 0.5);
			}
			if (selectDeck.getDoneSliding()) {
				pState = INPUT;
				selectDeck.setPlayable(true);
				//selectBtn.setVisible(true);
			}
		}
		else if (pState == INPUT) {
			if (selectDeck.hasSelected()) {
				selectBtn.setVisible(true);

			}
		}

		break;
	}
}
void GameScreen::ScreenDraw(float dt) {
	ClearBackground(GREEN);
	DrawTextEx(font, "Main Game", Vector2{ 100, 100 }, 16, 0, WHITE);

	DrawTexture(bg, 0, 0, WHITE);

	bossDeck.deckDraw();
	selectDeck.deckDraw();
	nextBtn.buttonDraw();
	selectBtn.buttonDraw();
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
		
		if (i <= 3) {
			deckSetup = false;
		}
	}
}


