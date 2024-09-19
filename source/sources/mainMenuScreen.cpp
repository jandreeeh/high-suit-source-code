#include "mainMenuScreen.h"


void MainMenuScreen::ScreenUpdate(float dt){
	//std::cout << "Im running\n";
}
void MainMenuScreen::ScreenDraw(float dt){
	ClearBackground(RED);
	DrawText("Main Menu", 100, 100, 10, WHITE);
}
int MainMenuScreen::getID() {
	return 20;
}
