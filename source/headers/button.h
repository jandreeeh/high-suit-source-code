#pragma once
#include <raylib.h>
#include <iostream>

#include "deck.h"


class Button {
private:
	Rectangle hitbox;
	Rectangle frame;
	Vector2 position;
	Vector2 origPos;
	std::string filename;
	Texture2D sprite;
	bool isSelected;
	bool isVisible;
	bool isHover;

public:
	Button(std::string filename, Vector2 position, bool visible);
	void onHover(Mouse mouse);
	void buttonInit();
	void buttonUpdate(float dt, Mouse mouse);
	void buttonDraw();
	void buttonDeInit();
	void setVisible(bool visible);
	bool getSelected();
	void setSelected(bool set);
};