#pragma once

#include "screen.h"

class MinigameScreen : public Screen {
public:
	void ScreenInit();
	void ScreenUpdate(float dt);
	void ScreenDraw(float dt);
};