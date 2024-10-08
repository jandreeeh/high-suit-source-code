#pragma once
class Transition {
private:
	bool isFadeIn = false;
	bool swap = false;
	float alpha = 0;
	float alpha2 = alpha;
	float time = 0;
public: 
	void fadeIn(float duration, float dt);
	void fadeOut(float duration, float dt);
	void transitionDraw();
	float getAlpha();
	bool getIsFadeIn();
};