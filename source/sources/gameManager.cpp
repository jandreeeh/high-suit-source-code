#include <raylib.h>
#include <iostream>

#include "gameManager.h"
#include "globals.h"

void GameManager::GameInit() {
    currentScreen->ScreenInit();
}

void GameManager::GameUpdate(float dt) {
    if (transitioning) {
        //std::cout << transitionDuration << "\n";
        if (transition.getAlpha() < 1.0f && nextScreen != nullptr) {
            transition.fadeIn(transitionDuration, dt);
        }
        else if (transition.getAlpha() == 1.0f && nextScreen != nullptr) {
            currentScreen->ScreenDeInit();  
            SetCurrentScreen(*nextScreen);
            currentScreen->ScreenInit(); 
            nextScreen = nullptr;
        }
        else if (transition.getAlpha() <= 0.0f && !transition.getIsFadeIn()) {
            transitioning = false;  
            return;
        }
        else if (nextScreen == nullptr) {
            transition.fadeOut(transitionDuration, dt);
        }
    }
    else {
        currentScreen->ScreenUpdate(dt);
        if (currentScreen == NULL) {
        }
    }
}

void GameManager::GameDraw() {
    currentScreen->ScreenDraw(GetFrameTime());
    

    if (transitioning) {
        transition.transitionDraw();
    }
}

void GameManager::GameDeInit() {
    currentScreen->ScreenDeInit();
}

void GameManager::SetCurrentScreen(Screen &screen) {
    currentScreen = &screen;
}

void GameManager::TransitionScreen(int screen, float d) {
    nextScreen = SetNextScreen(screen);
    transitioning = true;
    transitionDuration = d/2;
    std::cout << "TD: " << transitionDuration << "\n";
}

Screen* GameManager::SetNextScreen(int screen) {
    switch (screen) {
    case MAIN_MENU:
        return &mainMenuScreen;
        break;
    case MAIN_GAME:
        return &gameScreen;
        break;
   case MINIGAME:
       return &minigameScreen;
       break;
    }
}
