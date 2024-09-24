#pragma once
#include <iostream>
#include <raylib.h>
#include <vector>

struct Mouse {
	Vector2 position;
	Rectangle hitbox;
};

struct Card {
	int value;
	std::string face;
	std::string suit;
	std::string id;

	Card* next;
	Card* prev;
};

struct RenderCard{
	Vector2 position;
	Vector2 origPos;
	Vector2 newPos;
	Texture2D sprite;
	Rectangle hitbox;
	float time = 0;
	float scale = 1;
	bool stop = false;
	bool isHover;

	Card* card;
};

class Deck {
private:

	Card* head;
	Card* last;
	Card* selected;
	bool isSlide;
	bool isPlayable = false;


public:
	Deck(bool isPlayable);
	std::vector<RenderCard> hands;

	void deckUpdate(float dt, Mouse mouse);
	void deckDraw();

	void insertCard(std::string s, std::string f);
	void insertExisitingCard(Card* card);
	void insertJoker();
	void playCard();
	void removeCard(std::string id);
	void setSelected(std::string id);
	void displayDeck();

	void slideCards(Vector2 init, Vector2 final, float dt, bool toArrange);
	void isHover(Mouse mouse);
	Card* getSelected();
	Card* getAtIndex(int index);
	Card* transferCard(int index);
	Card* findCard(std::string id);

};

void FormGeneralDeck(Deck &deck);