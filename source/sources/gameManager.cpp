#include <iostream>
#include "raylib.h"

#include "reasing.h"
#include "gameManager.h"
	


void GameManager::GameInit() {
	currentScreen->ScreenInit();
}
void GameManager::GameUpdate() {
		currentScreen->ScreenUpdate(GetFrameTime());

}
void GameManager::GameDraw() {
	currentScreen->ScreenDraw(GetFrameTime());

	
}
void GameManager::GameDeInit() {
	currentScreen->ScreenDeInit();
}
void GameManager::SetCurrentScreen(Screen& screen) {
	currentScreen = &screen;
}
void GameManager::TransitionScreen(Screen &screen) {
	currentScreen = &screen;
}

