#pragma once

class Screen {
public:
	virtual void ScreenInit() {};
	virtual void ScreenUpdate(float dt) {};
	virtual void ScreenDraw(float dt) {};
	virtual void ScreenDeInit() {};
	virtual int getID() { return 0; };
};