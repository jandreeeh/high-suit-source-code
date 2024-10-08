#pragma once

class MGHandler {
private: 
	bool minigameWin;
	bool minigameDone;
public:
	bool getMinigameDone();
	bool getMinigameStats();
	void setMinigameDone(bool set);
	void setMinigameStats(bool set);
};
