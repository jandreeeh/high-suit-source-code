#pragma once

class Minigame {

public:
	virtual void MinigameInit();
	virtual void MinigameUpdate();
	virtual void MinigameDraw();
	virtual void MinigameDeInit();
};