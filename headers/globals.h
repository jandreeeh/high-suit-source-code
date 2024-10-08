#pragma once

enum states {
	MAIN_MENU,
	MAIN_GAME,
	MINIGAME
};

#define MAX(a, b) ((a)>(b)? (a) : (b)) //Used for virtual screen
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define VSCREEN_WIDTH 320
#define VSCREEN_HEIGHT 180

//Font font = LoadFont("source/resources/superstar.ttf");