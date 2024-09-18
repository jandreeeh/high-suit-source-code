#include <raylib.h>
#include <iostream>
#include <string>

#include "deck.h"

Deck::Deck() {
	head = NULL;
	last = NULL;
}

void Deck::insertCard(std::string s, std::string f) {
	Card* temp;
	int v;
	std::string id;

	if (f == "A") v = 1; 
	else if (f == "J") v = 11; 
	else if (f == "Q") v = 12; 
	else if (f == "K") v = 13; 
	else { v = stoi(f); }

	id = s + f;

	temp = new Card;
	temp->face = f;
	temp->suit = s;
	temp->value = v;
	temp->id = id;
	temp->next = NULL;
	temp->prev = NULL;

	if (head == NULL) { 
		head = temp; 
		last = head; 
	}
	else {
		last->next = temp;
		temp->prev = last;
		last = temp;
	}


}

void Deck::playCard() {

}

void Deck::removeCard(std::string id) {

}

void Deck::displayDeck() {
	if (head == NULL) {
		std::cout << "The deck is empty!" << std::endl;
		return;
	}

	Card* temp;
	temp = head;
	while (temp != NULL) { 
		std::cout << "Card ID: " << temp->id
			<< " | Suit: " << temp->suit
			<< " | Face: " << temp->face
			<< " | Value: " << temp->value << std::endl;
		temp = temp->next;
		//std::cout << "I worked (DD)" << std::endl;
	}
}



void FormGeneralDeck(Deck &deck) {
	std::string faces[] = { "A","2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
	std::string suits[] = { "D", "H", "S", "C" };
	std::cout << "I worked (FGD)" << std::endl;

	for (std::string s : suits) {
		for (std::string f : faces) {
			deck.insertCard(s, f);
		}
	}

}