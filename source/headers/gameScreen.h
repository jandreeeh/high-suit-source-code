#pragma once
#pragma once
#include <iostream>
#include <raylib.h>

#include "screen.h"

class GameScreen : public Screen {
public:
	void ScreenUpdate(float dt);
	void ScreenDraw(float dt);
};