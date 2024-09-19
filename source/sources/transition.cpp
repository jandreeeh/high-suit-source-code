#include <raylib.h>

#include "transistion.h"

void Transition::fadeIn(int duration, float dt) {
	alpha += dt;
	isFadeIn = true;
}
void Transition::fadeOut(int duration, float dt) {

}

void Transition::transitionDraw() {
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
}