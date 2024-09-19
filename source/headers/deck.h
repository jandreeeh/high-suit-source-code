#pragma once
#include <iostream>


struct Card {
	int value;
	std::string face;
	std::string suit;
	std::string id;

	Card* next;
	Card* prev;
};

class Deck {
private:

	Card* head;
	Card* last;
	Card* selected;

public:

	Deck();

	void insertCard(std::string s, std::string f);
	void playCard();
	void removeCard(std::string id);
	void displayDeck();
	void setSelected(std::string id);
	Card* getSelected();
	Card* findCard(std::string id);

};

void FormGeneralDeck(Deck &deck);