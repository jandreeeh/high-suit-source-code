#pragma once

#include "deck.h"

class House {
public: 
	RenderCard* bossPick;
	RenderCard* playerPick;
	void setBossPick(RenderCard card);
	void setPlayerPick(RenderCard card);

};