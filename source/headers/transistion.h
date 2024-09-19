#pragma once
class Transition {
private:
	bool isFadeIn = false;
	bool swap = false;
	float alpha = 0;
	float time = 0;
public: 
	void fadeIn(int duration, float dt);
	void fadeOut(int duration, float dt);
	void transitionDraw();
};