#include <raylib.h>
#include <iostream>
#include <string>

#include "deck.h"
#include "globals.h"
#include "reasing.h"

Deck::Deck(bool isPlayable) {
	this->head = NULL;
	this->last = NULL;
	this->selected = NULL;
	this->isSlide = false;
	this->isPlayable = isPlayable;
}
void Deck::deckUpdate(float dt, Mouse mouse) {
	//std::cout << "Mouse X: " << mouse.position.x << "\n";
	if (IsKeyPressed(KEY_ENTER)) { isSlide = true; }
	if (isSlide) {
		slideCards(Vector2{ ((VSCREEN_WIDTH / 2) - (30/2)), -40 }, Vector2{ 75, 20 }, dt, true);
	}
	else {

		if (isPlayable) {
			isHover(mouse);
		}

		for (int i = 0; i < hands.size(); i++) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hands[i].isHover) {
				selected = hands[i].card;
				std::cout << hands[i].card->id << " has been selected.\n";
			}

			if (hands[i].card == selected && !isSlide) {
				int newY = hands[i].origPos.y - 5;
				hands[i].position.y = newY;
			}

		}

	}

	
}

void Deck::deckDraw() {
	for (int i = 0; i < hands.size(); i++) {
		DrawTextureEx(hands[i].sprite, hands[i].position, 0, hands[i].scale, WHITE);
	}
}
void Deck::insertCard(std::string s, std::string f) {
	Card* temp;
	RenderCard tempH;
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

	insertExisitingCard(temp);
}

void Deck::insertExisitingCard(Card* card) {
	
	RenderCard tempH;

	std::string filename = "source/resources/sprites/cards/" + card->id + ".png";
	tempH.sprite = LoadTexture(filename.c_str());
	tempH.position.x = (VSCREEN_WIDTH / 2) - tempH.sprite.width / 2;
	tempH.position.y = 0 - tempH.sprite.height;
	tempH.origPos.x = tempH.position.x;
	tempH.origPos.y = tempH.position.y;
	tempH.hitbox.x = tempH.position.x;
	tempH.hitbox.y = tempH.position.y;
	tempH.hitbox.height = tempH.sprite.height;
	tempH.hitbox.width = tempH.sprite.width;
	tempH.card = card;

	hands.emplace_back(tempH);

	if (head == NULL) {
		head = card;
		last = head;
	}
	else {
		last->next = card;
		card->prev = last;
		last = card;
	}
	selected = NULL;
}

void Deck::insertJoker() {
	Card* temp = new Card;
	temp->face = "Joker";
	temp->suit = "Joker";
	temp->value = 20;
	temp->id = "joker";
	temp->next = NULL;
	temp->prev = NULL;

	insertExisitingCard(temp);
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

void Deck::setSelected(std::string id) {
	if (findCard(id) == nullptr) {
		selected = head;
		return;
	}
	selected = findCard(id);
}


bool setInit = true;

void Deck::slideCards(Vector2 init, Vector2 final, float dt, bool toArrange) {
	int width = VSCREEN_WIDTH - (2 * final.x);
	for (int i = 0; i < hands.size(); i++) {
		hands[i].time += dt;

		if(setInit){
			hands[i].position = init;
			hands[i].origPos = init;
			if (hands[hands.size() - 1].position.x == init.x && hands[hands.size() - 1].position.y == init.y) {
				setInit = false;
			}
			std::cout << "what\n";
		}
		else {
			if (toArrange) {
				hands[i].newPos.x = final.x + (width / hands.size()) * i;
				hands[i].newPos.y = final.y;
				//std::cout << i << " " << hands[i].newPos.x << ", " << hands[i].newPos.y <<"\n";

			}
			else {
				hands[i].newPos = final;

			}

			if (!hands[i].stop) {
				hands[i].position.x = EaseSineOut(hands[i].time, hands[i].origPos.x, hands[i].newPos.x - hands[i].origPos.x, 0.5f + (0.1 * i));
				hands[i].position.y = EaseSineOut(hands[i].time, hands[i].origPos.y, hands[i].newPos.y - hands[i].origPos.y, 0.5f + (0.1 * i));

				if (hands[i].position.x >= hands[i].newPos.x && hands[i].position.y >= final.y){
					hands[i].stop = true; 
					hands[i].origPos.x = hands[i].position.x;
					hands[i].origPos.y = hands[i].position.y;
					hands[i].hitbox.x = hands[i].position.x;
					hands[i].hitbox.y = hands[i].position.y;
				}

			}
		}
		if (hands[hands.size() - 1].stop) {
			isSlide = false;
		}
	}
}

void Deck::isHover(Mouse mouse) {
	for (int i = 0; i < hands.size(); i++) {
		if (hands[i].stop) {
			int newY = hands[i].origPos.y - 3;
			if (CheckCollisionRecs(hands[i].hitbox, mouse.hitbox)) {
				hands[i].position.y = newY;
				hands[i].isHover = true;
				//std::cout << "Im hovering " << hands[i].card->id << "\n";
			}
			else {
				hands[i].position.y = hands[i].origPos.y;
				hands[i].isHover = false;
			}
		}
	}
}

Card* Deck::getSelected() {
	return selected;
}

Card* Deck::getAtIndex(int index) {
	Card* temp = head;
	for (int i = 0; i < index; i++) {
		if(temp->next != nullptr) temp = temp->next;
	}
	return temp;
}

Card* Deck::transferCard(int index) {
	Card* temp = getAtIndex(index);
	if (temp->prev != nullptr) { temp->prev->next = temp->next;}
	if (temp->next != nullptr) temp->next->prev = temp->prev;
	if (temp == head) head = head->next;

	temp->prev = temp->next = nullptr;
	return temp;
}

Card* Deck::findCard(std::string id) {
	if (head == NULL) {
		std::cout << "The deck is empty!" << std::endl;
		return nullptr;
	}
	Card* temp = head;
	while (temp != NULL) {
		if (id == temp->id) {
			return temp;
		}
		temp = temp->next;
	}
	if (temp == nullptr){
		return nullptr;
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