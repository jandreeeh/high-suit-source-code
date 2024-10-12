#pragma once

#include "screen.h"

class GameScreen : public Screen {
public:
	void ScreenInit();
	void ScreenDeInit();
	void ScreenUpdate(float dt);
	void ScreenDraw(float dt);
	int getID();
};