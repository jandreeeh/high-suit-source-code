#include "gameScreen.h"

#include <iostream>
#include <raylib.h>

#include "screen.h"
#include "globals.h"
#include "game.h"
#include "deck.h"


static Deck mainDeck(false);
static Deck playerDeck(true);
static Deck bossDeck(false);
static Mouse mouse;
static int totaldeck = 52;
static int setup = true;
static float scale;


void GameScreen::ScreenInit() {
	if (setup) {
		FormGeneralDeck(mainDeck);

		for (int i = 0; i < 5; i++) {
			int index = GetRandomValue(0, totaldeck);
			std::cout << index << "\n";
			Card* temp = mainDeck.transferCard(index);
			bossDeck.insertExisitingCard(temp);
			totaldeck--;
		}
		setup = false;
	}
	bossDeck.displayDeck();

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

	bossDeck.deckUpdate(dt, mouse);
}
void GameScreen::ScreenDraw(float dt) {
	ClearBackground(GREEN);
	//DrawText(TextFormat("Mouse X: %04i", mouseX), 50, 25,10, WHITE );
	//DrawText(TextFormat("Mouse Y: %04i", mouseY), 50, 50, 10, WHITE);
	DrawText("Gaming Time", 100, 100, 10, WHITE);
	bossDeck.deckDraw();
}
int GameScreen::getID() { 
	return 10; 
}
