#include <raylib.h>
#include <reasing.h>
#include <iostream>

#include "transition.h"

int framecounter = 0;

void Transition::fadeIn(float duration, float dt) {
    if (alpha < 1.0f) {
        time += dt;
        framecounter++;
        //std::cout << time << " FI Time" << std::endl;
        //std::cout << framecounter << " FI Frame" << std::endl;

        alpha = EaseLinearIn(time, alpha2, 1.0f - alpha2, duration);
        if (alpha >= 1.0f) {
            alpha = 1.0f;
            alpha2 = alpha;
            time = 0;
            framecounter = 0;
        }
    }
    isFadeIn = true;
}

void Transition::fadeOut(float duration, float dt) {
    if (alpha > 0.0f) {
        time += dt;
        framecounter++;
        //std::cout << time << " FO TIme" << std::endl;
        //std::cout << framecounter << " FO Frame" << std::endl;
        alpha = EaseLinearOut(time, alpha2, 0 - alpha2, duration);
        if (alpha < 0.0f) {
            alpha = 0.0f;
            alpha2 = alpha;
            time = 0;
            framecounter = 0;
        }
    }
    isFadeIn = false;
}

void Transition::transitionDraw() {
    if (alpha > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
    }
}
float Transition::getAlpha() { return alpha; }

bool Transition::getIsFadeIn() { return isFadeIn; }