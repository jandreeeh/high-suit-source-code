#pragma once

#include "screen.h"
#include "mainMenuScreen.h"
#include "gameScreen.h"

class GameManager {
private:
	Screen* currentScreen = nullptr;

public:
	void GameInit();
	void GameUpdate();
	void GameDraw();
	void GameDeInit();
	void SetCurrentScreen(Screen& screen);
	void TransitionScreen(Screen &screen);
};
