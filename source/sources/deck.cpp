#include <raylib.h>
#include <iostream>
#include <string>
#include <cmath>

#include "deck.h"
#include "globals.h"
#include "reasing.h"

float getDistance(Vector2 vec1, Vector2 vec2) {
	float dx = vec2.x - vec1.x;
	float dy = vec2.y - vec1.y;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}

Deck::Deck(bool isPlayable) {
	this->head = NULL;
	this->last = NULL;
	this->selected = NULL;
	this->isSlide = false;
	this->isPlayable = isPlayable;
}

void Deck::deckUpdate(float dt, Mouse mouse) {
	//std::cout << "Mouse X: " << mouse.position.x << "\n";
	// maglaban si boss fight

	if (isPlayable) {
		isHover(mouse);
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

	if (isHidden) {
		RenderCard temp;
		for (int i = 0; i < hands.size(); i++) {
			if (selected = hands[i].card) {
				temp = hands[i];
			}
		}

		back.position = temp.position;
	}

}

void Deck::deckDraw() {
	for (int i = 0; i < hands.size(); i++) {
		DrawTextureEx(hands[i].sprite, hands[i].position, 0, hands[i].scale, WHITE);
	}
	if (isHidden) {
		DrawTextureEx(back.sprite, back.position, 0, 1, WHITE);

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
	tempH.position.x = 0 - tempH.sprite.width;
	tempH.position.y = VSCREEN_HEIGHT/2;
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
	std::cout << "Card ID: " << card->id
		<< " | Suit: " << card->suit
		<< " | Face: " << card->face
		<< " | Value: " << card->value << std::endl;
}

void Deck::insertSelectedExisitingCard(Card* card) {

	RenderCard tempH;

	std::string filename = "source/resources/sprites/cards/" + card->id + ".png";
	tempH.sprite = LoadTexture(filename.c_str());
	tempH.position.x = 0 - tempH.sprite.width;
	tempH.position.y = VSCREEN_HEIGHT / 2;
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
		card->prev = nullptr;
		card->next = nullptr;
		std::cout << "no head\n";
	}
	else {
		last->next = card;
		card->prev = last;
		last = card;
	}
	selected = head;
	std::cout << "Card ID: " << selected->id
		<< " | Suit: " << selected->suit
		<< " | Face: " << selected->face
		<< " | Value: " << selected->value << std::endl;
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

void Deck::displayRenderDeck() {
	if (!hands.empty()) {
		for (int i = 0; i < hands.size(); i++) {
			Card* temp;
			temp = hands[i].card;
			std::cout << "Card ID: " << temp->id
					<< " | Suit: " << temp->suit
					<< " | Face: " << temp->face
					<< " | Value: " << temp->value << std::endl;
		}
	}
	else {
		std::cout << "Deck Empty\n";
	}
}


void Deck::clearDeck() {
	Card* current = head;
	while (current != nullptr) {
		Card* nextCard = current->next;
		delete current; 
		current = nextCard;
	}
	if (!hands.empty()) {
		for (int i = 0; i < hands.size() ; i++) {
			UnloadTexture(hands[i].sprite);

		}
		hands.clear();	
	}

	head = nullptr;
	last = nullptr;
	selected = nullptr;

	std::cout << "Deck has been cleared." << std::endl;
}

void Deck::setSelected(std::string id) {
	if (findCard(id) == nullptr) {
		selected = head;
		return;
	}
	selected = findCard(id);
}

void Deck::setPlayable(bool b) {
	isPlayable = b;
}

bool Deck::hasSelected() {
	if (selected != nullptr) return true;
	else { return false; }
}

static bool setInit = true;

void Deck::slideCards(Vector2 init, Vector2 final, float dt, bool toArrange, bool keepPos, float duration) {
	int width = (VSCREEN_WIDTH  - (2 * final.x)) - hands[0].sprite.width;
	doneSlide = false;

	if (setInit) {
		for (int i = 0; i < hands.size(); i++) {
			hands[i].stop = false;
			hands[i].time = 0.0f;

			if (!keepPos) {

				// Set initial position only if we are not keeping the current position
				hands[i].position = init;
				hands[i].origPos = init;
			}
			else {
				hands[i].origPos = hands[i].position;
			}
			if (toArrange) {
				if (hands.size() == 1) { hands[i].newPos.x = final.x + width / 2; }
				else { hands[i].newPos.x = final.x + ((width / (hands.size() - 1)) * i); }

				hands[i].newPos.y = final.y;
			}
			else {
				hands[i].newPos = final;
			}
		}
		// Set init state to false after initialization is complete
		setInit = false;
	}

	for (int i = 0; i < hands.size(); i++) {
		if (!hands[i].stop) {
			//std::cout << i << ": " << getDistance(hands[i].position, hands[i].newPos) << "\n";

			// Update position over time using easing function
			hands[i].position.x = EaseQuadInOut(hands[i].time, hands[i].origPos.x, hands[i].newPos.x - hands[i].origPos.x, duration + (0.1f * i));
			hands[i].position.y = EaseQuadInOut(hands[i].time, hands[i].origPos.y, hands[i].newPos.y - hands[i].origPos.y, duration + (0.1f * i));

			// Increase time for the card
			hands[i].time += GetFrameTime();

			// Stop sliding if the card reaches its target position
			if (getDistance(hands[i].position, hands[i].newPos) <= 0.1f) {
				hands[i].stop = true;
				hands[i].origPos = hands[i].position;
				hands[i].hitbox.x = hands[i].position.x;
				hands[i].hitbox.y = hands[i].position.y;
			}
		}
	}

	// Optional: Check if all cards have finished sliding
	bool allCardsStopped = true;
	for (int i = 0; i < hands.size(); i++) {
		if (!hands[i].stop) {
			allCardsStopped = false;
			break;
		}
	}
	if (allCardsStopped) {
		std::cout << "All cards have completed sliding.\n";
		// Reset flags or perform any post-animation logic if needed
		isSlide = false;
		doneSlide = true;
		setInit = true;
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

void Deck::initBackCard() {
	back.sprite = LoadTexture("source/resources/sprites/cards/back.png");
	back.position = Vector2{ -100, -100 };
}
void Deck::hideSelected(bool set) {
	isHidden = set;
}

bool Deck::getSliding() {
	return isSlide;
}

void Deck::setSliding(bool set) {
	isSlide = set;
}

bool Deck::getDoneSliding() {
	return doneSlide;
}

void Deck::setDoneSliding(bool set) {
	doneSlide = set;
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

	for (int i = 0; i < hands.size(); i++) {
		if (hands[i].card == temp) {
			UnloadTexture(hands[i].sprite);
			hands.erase(hands.begin() + i);
			break;
		}
	}

	if (temp == head) head = head->next;
	if (temp == last) last = last->prev;
	if (temp->prev != nullptr) { temp->prev->next = temp->next; }
	if (temp->next != nullptr) temp->next->prev = temp->prev;

	temp->prev = temp->next = nullptr;
	return temp;
}

Card* Deck::transferSelectedCard() {
	Card* temp = getSelected();
	if (!hands.empty()) {
		for (int i = 0; i < hands.size(); i++) {
			if (hands[i].card == temp) {
				UnloadTexture(hands[i].sprite);
				hands.erase(hands.begin() + i);
				break;

			}
		}

	}

	if (temp != nullptr) {
		if (temp == head) head = head->next;
		if (temp == last) last = last->prev;
		if (temp->prev != nullptr) { temp->prev->next = temp->next; }
		if (temp->next != nullptr) temp->next->prev = temp->prev;

		temp->prev = temp->next = nullptr;
		selected = nullptr;
		return temp;
	}
	else {
		return nullptr;
	}
}
Card* Deck::transferRandomCard() {
	int index = GetRandomValue(0, hands.size() - 1);  // Select a random index
	Card* temp = hands[index].card;  // Get the card from the random index

	if (!hands.empty()) {
		UnloadTexture(hands[index].sprite);
		hands.erase(hands.begin() + index);
	}

	if (temp != nullptr) {
		// Update head and last pointers
		if (temp == head) {
			head = head->next;
		}
		if (temp == last) {
			last = last->prev;
		}

		// Safely update prev and next links
		if (temp->prev != nullptr) {
			temp->prev->next = temp->next;
		}
		if (temp->next != nullptr) {
			temp->next->prev = temp->prev;
		}

		// Reset pointers for the removed card
		temp->prev = temp->next = nullptr;
		selected = nullptr;

		return temp;
	}

	return nullptr;
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