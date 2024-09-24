#include "button.h"

#include "game.h"


Button::Button(std::string filename, Vector2 position, bool visible) {
	this->filename = "source/resources/sprites/buttons/" + filename + ".png";
	this->position = position;
	this->origPos = position;
	this->isVisible = visible;
	this->isSelected = false;
	this->isHover = false;

}
void Button::buttonInit() {
	sprite = LoadTexture(filename.c_str());
	hitbox = Rectangle{ position.x, position.y, (float)sprite.width, (float)sprite.height };
	frame = Rectangle{ 0.0f, 0.0f, (float)sprite.width / 2, (float)sprite.height };
}
void Button::buttonUpdate(float dt, Mouse mouse) {
	if (isVisible) {
		onHover(mouse);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isHover) {
			isSelected = true;
			std::cout << filename << " is selected\n";
		}
	}
}

void Button::buttonDraw() {
	if (isVisible) {
		DrawTextureRec(sprite, frame, position, WHITE);
	}
}

void Button::onHover(Mouse mouse) {
	int newX = origPos.x + 5;
	if (CheckCollisionRecs(hitbox, mouse.hitbox)) {
		position.x = newX;
		frame.x = sprite.width / 2;
		isHover = true;
	}
	else {
		position.x = origPos.x;
		frame.x = 0;
		isHover = false;
	}
}

bool Button::getSelected() {
	return isSelected;
}

void Button::setVisible(bool visible) {
	isVisible = visible;
}