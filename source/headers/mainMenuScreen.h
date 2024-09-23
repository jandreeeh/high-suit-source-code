#pragma once

#include "screen.h"


class MainMenuScreen : public Screen {
public:
	void ScreenUpdate(float dt);
	void ScreenDraw(float dt);
	int getID();
};