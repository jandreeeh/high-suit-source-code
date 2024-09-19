#include "gameScreen.h"


void GameScreen::ScreenUpdate(float dt) {

}
void GameScreen::ScreenDraw(float dt) {
	ClearBackground(GREEN);
	DrawText("Gaming Time", 100, 100, 10, WHITE);
}
int GameScreen::getID() { 
	return 10; 
}
