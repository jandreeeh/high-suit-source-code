#pragma once

#include "transition.h"
#include "screen.h"
#include "mainMenuScreen.h"
#include "gameScreen.h"

class GameManager {
private:
	MainMenuScreen mainMenuScreen;
	GameScreen gameScreen;

	Screen* currentScreen = &mainMenuScreen;
	Screen* nextScreen = nullptr;
	Transition transition;
	bool transitioning = false;
	float transitionDuration = 0;

public:
	void GameInit();
	void GameUpdate(float dt);
	void GameDraw();
	void GameDeInit();
	void SetCurrentScreen(Screen& screen);
	void TransitionScreen(int screenscreen, float duration);
	Screen* SetNextScreen(int screen);
};
