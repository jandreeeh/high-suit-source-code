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

public:

	Card* head;
	Card* last;
	Deck();

	void insertCard(std::string s, std::string f);
	void playCard();
	void removeCard(std::string id);
	void displayDeck();

};

void FormGeneralDeck(Deck &deck);