#include "gameScreen.h"

#include <iostream>
#include <raylib.h>

#include "screen.h"
#include "globals.h"
#include "game.h"
#include "deck.h"
#include "button.h"

static enum stages {
	BEGIN = 90,
	DECK_BUILD = 91,
	BOSS_BATTLE = 92,
	END_GAME = 93
};
static enum playerState {
	CUTSCENE,
	INPUT,
	HYPE,
	REVEAL,
	RESOLVE
};

static Deck mainDeck(false);
static Deck bossDeck(false);
static Deck playerDeck(false);
static Deck selectDeck(false);

static Deck bossBattle(false);
static Deck playerBattle(false);

static Mouse mouse;
static Button nextBtn("next", Vector2{ 262, 125 }, true);
static Button selectBtn("select", Vector2{250, 115}, true);

static Texture2D bg;
static Texture2D winFlash;
static Texture2D loseFlash;
static float winAlpha = 0;
static float loseAlpha = 0;


static Camera2D camera;

static int totaldeck = 52;
static bool setup = true;
static bool deckSetup = true;
static bool jokerIn = true;
static int deckBuildStage = 0;
static float scale;

static float gameClock;

static stages currentStage;
static playerState pState;

//Deck Build Logic
static bool hasSelected = false;
static bool selectSlideOut = false;
//Boss Battle Logic
static bool showCard = false;
static bool hidePCard = true;
static bool drumroll = true;
static bool playerRoundWin = false;
static bool firstCardOut = false;
static bool flashBool = false;
static bool bossCardsHidden = false;
static bool endGame = false;

static int playerCards = 0;
static int bossCards = 5;

static int playerPoints = 0;
static int bossPoints = 0;

static int currentRound = 0;

//End Game Logic
static bool wonGame = false;

void setRandomCards();
void setOneRandomCard();
int resolveBattle(Card* playerCard, Card* bossCard);

void GameScreen::ScreenInit() {
	nextBtn.setVisible(false);
	selectBtn.setVisible(false);
	deckSetup = true;
	bg = LoadTexture("source/resources/sprites/bg/table.png");
	winFlash = LoadTexture("source/resources/sprites/bg/win.png");
	loseFlash = LoadTexture("source/resources/sprites/bg/lose.png");

	camera = { 0 };
	hasSelected = false;
	selectSlideOut = false;

	camera.offset = Vector2{ VSCREEN_WIDTH / 2, VSCREEN_HEIGHT / 2 };
	camera.target = Vector2{ VSCREEN_WIDTH / 2, VSCREEN_HEIGHT / 2 };
	camera.zoom = 1;
	camera.rotation = 0;

	if (currentStage == DECK_BUILD && deckSetup) {
		if (MinigameHandler.getMinigameStats()) {
			setRandomCards();
		}
		else {
			setOneRandomCard();
		}
	}

	if (setup) {
		currentStage = BEGIN;
		pState = CUTSCENE;
		gameClock = 0;
		bossCards = 5;

		FormGeneralDeck(mainDeck);
		int index;

		for (int i = 0; i < 5; i++) {
			index = GetRandomValue(0, 8);
			if (index == 2 && jokerIn) {
				bossDeck.insertJoker();
				jokerIn = false;
			}
			else {
				index = GetRandomValue(0, totaldeck);
				std::cout << index << "\n";
				bossDeck.insertExisitingCard(mainDeck.transferCard(index));
				totaldeck--;
			}
		}
		nextBtn.buttonInit();
		selectBtn.buttonInit();
		setup = false;

		bossBattle.initBackCard();

		playerPoints = 0;
		bossPoints = 0;
		currentRound = 0;

		hasSelected = false;
		selectSlideOut = false;

		showCard = false;
		hidePCard = true;
		drumroll = true;
		playerRoundWin = false;
		firstCardOut = false;
		flashBool = false;
		bossCardsHidden = false;
		endGame = false;
	}



	scale = MIN((float)GetScreenWidth() / VSCREEN_WIDTH, (float)GetScreenHeight() / VSCREEN_HEIGHT);
	mouse.hitbox.width = 1;
	mouse.hitbox.height = 1;
}

void GameScreen::ScreenUpdate(float dt) {
	//Screen mouse position to virtual screen position
	mouse.position.x = (GetMouseX() - (GetScreenWidth() - (VSCREEN_WIDTH * scale)) * 0.5f) / scale;
	mouse.position.y = (GetMouseY() - (GetScreenHeight() - (VSCREEN_HEIGHT * scale)) * 0.5f) / scale;
	mouse.hitbox.x = mouse.position.x;
	mouse.hitbox.y = mouse.position.y;

	camera.offset = Vector2{ VSCREEN_WIDTH / 2, VSCREEN_HEIGHT / 2 };
	camera.target = Vector2{ VSCREEN_WIDTH / 2, VSCREEN_HEIGHT / 2 };

	nextBtn.buttonUpdate(dt, mouse);
	selectBtn.buttonUpdate(dt, mouse);
	bossDeck.deckUpdate(dt, mouse);
	selectDeck.deckUpdate(dt, mouse);
	playerDeck.deckUpdate(dt, mouse);
	bossBattle.deckUpdate(dt, mouse);
	playerBattle.deckUpdate(dt, mouse);


	switch (currentStage) {
	case BEGIN:
		if (pState == CUTSCENE) {
			gameClock += dt;
			if (gameClock > 0.5) { bossDeck.setSliding(true); }

			if (bossDeck.getSliding()) {
				bossDeck.slideCards(Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), -40 }, Vector2{ 70, 15 }, dt, true, false, 0.5);
			}
			if (bossDeck.getDoneSliding()) {
				bossDeck.displayRenderDeck();
				bossDeck.setDoneSliding(false);
				pState = INPUT;
				nextBtn.setVisible(true);
			}
		}
		else if (pState == INPUT) {
			if (nextBtn.getSelected()) {
				nextBtn.setSelected(false);
				pState = CUTSCENE;
				currentStage = DECK_BUILD;
				GM.TransitionScreen(MINIGAME, 0.5);
			}
		}
		break;

	case DECK_BUILD:
		if (pState == CUTSCENE) {
			if (!hasSelected) {
				gameClock += dt;
				// Check the minigame result
				if (MinigameHandler.getMinigameStats()) {
					// Player won, let them select 3 cards
					if (gameClock > 0.5) { selectDeck.setSliding(true); }

					if (selectDeck.getSliding()) {
						selectDeck.slideCards(Vector2{ VSCREEN_WIDTH + 10, VSCREEN_HEIGHT / 2 }, Vector2{ 100, VSCREEN_HEIGHT / 2 }, dt, true, false, 0.5);
					}
					if (selectDeck.getDoneSliding()) {
						selectDeck.setPlayable(true);
						selectDeck.setDoneSliding(false);

						gameClock = 0;
						pState = INPUT;
					}
				}
				else {
					// Player lost, get one random card with sweeping animation
					if (!selectSlideOut) {
						if (gameClock > 0.5) { selectDeck.setSliding(true); }

						if (selectDeck.getSliding()) {
							selectDeck.slideCards(Vector2{ VSCREEN_WIDTH + 10, VSCREEN_HEIGHT / 2 }, Vector2{ -100, VSCREEN_HEIGHT / 2 }, dt, false, false, 1);
						}
						if (selectDeck.getDoneSliding()) {
							playerDeck.setSliding(true);
							selectDeck.setDoneSliding(false);

							gameClock = 0;
							pState = INPUT;
							
							//break;
						}
					}
				}
			}
			else {
				// Handle when a card has been selected (similar to original logic)
				if (!selectSlideOut) {
					if (MinigameHandler.getMinigameStats()) {
						if (selectDeck.getSliding()) {
							selectDeck.slideCards(Vector2{ 100, VSCREEN_HEIGHT / 2 }, Vector2{ -100, VSCREEN_HEIGHT / 2 }, dt, false, true, 0.5);
						}

						if (selectDeck.getDoneSliding()) {
							//std::cout << "insert running 1\n";

							playerDeck.insertExisitingCard(selectDeck.transferSelectedCard());
							playerDeck.setSliding(true);
							selectDeck.setDoneSliding(false);

							selectSlideOut = true;
						}
					}
					else {
						//std::cout << "insert running 2\n";

						playerDeck.insertExisitingCard(selectDeck.transferSelectedCard());
						playerDeck.setSliding(true);
						selectSlideOut = true;
					}
				}

				if (playerDeck.getSliding()) {
					//std::cout << "insert running 3\n";

					playerDeck.slideCards(Vector2{ -100, (VSCREEN_HEIGHT / 2) + 45 }, Vector2{ 70, (VSCREEN_HEIGHT / 2) + 45 }, dt, true, false, 0.5);
				}
				if (playerDeck.getDoneSliding()) {
					pState = INPUT;
					playerDeck.setDoneSliding(false);
					if (playerCards != 5) nextBtn.setVisible(true);
					else { 
						currentStage = BOSS_BATTLE;
						pState = CUTSCENE;
					}
				}
			}
		}
		else if (pState == INPUT) {
			// Handle card selection input and transitions
			if (!selectSlideOut) {
				if (MinigameHandler.getMinigameStats()) {
					if (selectDeck.hasSelected()) {
						selectBtn.setVisible(true);
					}
					if (selectBtn.getSelected() && selectDeck.hasSelected()) {
						selectDeck.setDoneSliding(false);
						selectDeck.setSliding(true);
						selectBtn.setVisible(false);
						selectBtn.setSelected(false);
						hasSelected = true;
						pState = CUTSCENE;
						playerCards++;

					}

				}
				else {
					hasSelected = true;
					pState = CUTSCENE;
					playerCards++;

				}
			}

			if (nextBtn.getSelected()) {
				nextBtn.setSelected(false);
				pState = CUTSCENE;
				currentStage = DECK_BUILD;

				selectDeck.clearDeck();
				GM.TransitionScreen(MINIGAME, 0.5);
			}
			if (playerCards >= 5) {
				//currentStage = BOSS_BATTLE;

			}
		}
		break;
	case BOSS_BATTLE:
		if (pState == CUTSCENE) {
			gameClock += dt;
			if (gameClock > 2) { 
				bossDeck.setSliding(true); 
			}

			if (!showCard) {
				if (!bossCardsHidden) {
					if (bossDeck.getSliding()) {
						bossDeck.slideCards(Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), -100 }, Vector2{ 75, -50 }, dt, true, true , 1);
					}

					if (bossDeck.getDoneSliding()) {
						bossDeck.setDoneSliding(false);
						bossBattle.insertSelectedExisitingCard(bossDeck.transferRandomCard());
						/*std::cout << "Boss Deck 1\n";
						bossDeck.displayDeck();*/

						bossBattle.hideSelected(true);
						bossBattle.setSliding(true);
						showCard = true;
						bossCardsHidden = true;
						//pState = INPUT;
						//nextBtn.setVisible(true);
					}
				}
				else {
					playerDeck.setSliding(true);

					if (playerDeck.getSliding()) {
						playerDeck.slideCards(Vector2{ -100, (VSCREEN_HEIGHT / 2) + 45 }, Vector2{ 70, (VSCREEN_HEIGHT / 2) + 45 }, dt, true, true, 0.5);
					}
					if (playerDeck.getDoneSliding()) {
						playerDeck.setDoneSliding(false);
						bossBattle.insertSelectedExisitingCard(bossDeck.transferRandomCard());
						/*std::cout << "Boss Deck 2\n";
						bossDeck.displayDeck();*/
						bossBattle.hideSelected(true);
						bossBattle.setSliding(true);
						showCard = true;
						//bossCardsHidden = true;
					}

				}

			}
			else {
				if (bossBattle.getSliding()) {
					bossBattle.slideCards(Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), -40 }, Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), 35 }, dt, false , false, 1);

				}
				if (bossBattle.getDoneSliding()) {
					playerDeck.setPlayable(true);
					bossBattle.setDoneSliding(false);
					showCard = false;
					pState = INPUT;
					gameClock = 0;

				}

			}
		}
		else if (pState == INPUT) {
			if (playerDeck.hasSelected()) {
				selectBtn.setVisible(true);
			}
			if (selectBtn.getSelected() && playerDeck.hasSelected()) {
				selectBtn.setSelected(false);
				selectBtn.setVisible(false);
				playerDeck.setPlayable(false);
				playerDeck.setSliding(true);
				pState = HYPE;
			}

		}
		else if (pState == HYPE) {
			gameClock += dt;
			if (hidePCard) {
				if (playerDeck.getSliding()) {
					playerDeck.slideCards(Vector2{0, 0}, Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), VSCREEN_HEIGHT + 45 }, dt, true, true, 0.5);
				}
				if (playerDeck.getDoneSliding()) {
					playerDeck.setDoneSliding(false);
					playerBattle.insertSelectedExisitingCard(playerDeck.transferSelectedCard());
					/*std::cout << "Player Deck 2\n";
					playerDeck.displayDeck();*/
					hidePCard = false;
					gameClock = 0;
					playerBattle.setSliding(true);
				}

			}
			else {
				if (playerBattle.getSliding()) {
					playerBattle.slideCards(Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), VSCREEN_HEIGHT + 40 }, Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), 95 }, dt, false, false, 1);
				}
				if (playerBattle.getDoneSliding()) {
					playerBattle.setDoneSliding(false);
					//hidePCard = false;
					gameClock = 0;
					hidePCard = true;
					pState = REVEAL;
					//playerBattle.setSliding(true);
				}
			}

		}
		else if (pState == REVEAL) {
			gameClock += dt;
			if (drumroll) {
				camera.zoom += dt * 0.25f;

				if (gameClock > 3.0f) {
					drumroll = false;
					gameClock = 0;
					camera.zoom = 1;
					bossBattle.hideSelected(false);
				}
			}
			else {
				int verdict = resolveBattle(playerBattle.getSelected(), bossBattle.getSelected());
				if (verdict == 1) {
					playerPoints++;
					playerRoundWin = true;
				}
				else {
					playerRoundWin = false;
					bossPoints++;
				}


				if (gameClock > 1) { 
						currentRound++;
					if (currentRound < 5) {
						std::cout << "Round " << currentRound << "\n";
						drumroll = true;
						pState = RESOLVE;
						gameClock = 0;
						
					}
					else {
						endGame = true;
						pState = RESOLVE;
						gameClock = 0;
					}
				}

			}
		}
		else if (pState == RESOLVE) {
			gameClock += dt;
			
			if (playerRoundWin) {
				if (gameClock > 0.5f && !flashBool) {
					bossBattle.setSliding(true);
					winAlpha = 1;
					flashBool = true;
				}
				winAlpha -= dt * 5.0f;
				if (!firstCardOut) {
					if (bossBattle.getSliding()) {
						bossBattle.slideCards(Vector2{ 0, 0 }, Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), VSCREEN_HEIGHT + 40 }, dt, false, true, 0.25f);
					}
					if (bossBattle.getDoneSliding()) {
						bossBattle.setDoneSliding(false);
						playerBattle.setSliding(true);
						firstCardOut = true;
					}
				}
				else {
					if (playerBattle.getSliding()) {
						playerBattle.slideCards(Vector2{0, 0 }, Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), VSCREEN_HEIGHT + 40 }, dt, false, true, 0.25f);
					}
					if (playerBattle.getDoneSliding()) {
						playerBattle.setDoneSliding(false);
						bossBattle.clearDeck();
						playerBattle.clearDeck();
						flashBool = false;
						firstCardOut = false;
						/*std::cout << "expected\n";

						std::cout << "Player Deck 1\n";
						playerDeck.displayRenderDeck();
						std::cout << "\n";

						std::cout << "Boss Deck 1\n";
						bossDeck.displayRenderDeck();
						std::cout << "\n";*/

						if (endGame) { 
							std::cout << "Yippee\n";
							currentStage == END_GAME;
						}
						else { 
							pState = CUTSCENE; 
						}
					}
				}
				
			}
			else {
				if (gameClock > 0.5f && !flashBool) {
					playerBattle.setSliding(true);
					loseAlpha = 1;
					flashBool = true;
				}
				loseAlpha -= dt * 5.0f;

				if (!firstCardOut) {
					if (playerBattle.getSliding()) {
						playerBattle.slideCards(Vector2{ 0, 0 }, Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)), -75 }, dt, false, true, 0.25f);
					}
					if (playerBattle.getDoneSliding()) {
						playerBattle.setDoneSliding(false);
						bossBattle.setSliding(true);
						firstCardOut = true;
					}
				}
				else{
					if (bossBattle.getSliding()) {
						bossBattle.slideCards(Vector2{ 0, 0 }, Vector2{ ((VSCREEN_WIDTH / 2) - (30 / 2)),- 75 }, dt, false, true, 0.25f);

					}
					if (bossBattle.getDoneSliding()) {
						bossBattle.setDoneSliding(false);
						bossBattle.clearDeck();
						playerBattle.clearDeck();
						firstCardOut = false;
						flashBool = false;
						/*std::cout << "expected\n";

						std::cout << "Player Deck 2\n";
						playerDeck.displayRenderDeck();
						std::cout << "\n";

						std::cout << "Boss Deck 2\n";
						bossDeck.displayRenderDeck();
						std::cout << "\n";*/

						if (endGame) {
							std::cout << "Yippee\n";
							currentStage == END_GAME;
						}
						else {
							pState = CUTSCENE;
						}
						
					}
				}
			}

		}
		break; 
	case END_GAME:
		if (playerPoints > bossPoints) {
			wonGame = true;
		}
		else {
			wonGame = false;
		}

		break;
	}
}

void GameScreen::ScreenDraw(float dt) {
	ClearBackground(GREEN);

	BeginMode2D(camera);

	DrawTexture(bg, 0, 0, WHITE);

	bossDeck.deckDraw();
	playerBattle.deckDraw();
	selectDeck.deckDraw();
	playerDeck.deckDraw();
	bossBattle.deckDraw();

	EndMode2D();
	nextBtn.buttonDraw();
	selectBtn.buttonDraw();
	
	if (currentStage == BOSS_BATTLE) {
		DrawTexture(winFlash, 0, 0, Fade(WHITE, winAlpha));
		DrawTexture(loseFlash, 0, 0, Fade(WHITE, loseAlpha));
	}
	else if (currentStage == END_GAME) {
		if (wonGame) {
			DrawRectangle(0, 0, VSCREEN_WIDTH, VSCREEN_HEIGHT, Fade(RED, 0.5f));
			DrawTextEx(font, "YOU LOSE!", Vector2{ ((float)VSCREEN_WIDTH / 2) - (MeasureTextEx(font,"YOU LOSE!", 32, 0).x / 2), VSCREEN_HEIGHT / 2 - 30 }, 32, 0, RED);
			DrawTextEx(font, "Press Left Mouse Button to Return to menu", Vector2{ (float)VSCREEN_WIDTH / 2 - MeasureTextEx(font,"Press Left Mouse Button to Return to menu", 16, 0).x / 2, VSCREEN_HEIGHT / 2 }, 16, 0, RAYWHITE); // Change 20 to 10
		}
		else {
			DrawRectangle(0, 0, VSCREEN_WIDTH, VSCREEN_HEIGHT, Fade(DARKGREEN, 0.5f));
			DrawTextEx(font, "YOU WIN!", Vector2{ (float)VSCREEN_WIDTH / 2 - MeasureTextEx(font,"YOU WIN!", 32, 0).x / 2, VSCREEN_HEIGHT / 2 - 30 }, 32, 0, GREEN);
			DrawTextEx(font, "Press Left Mouse Button to Return to menu", Vector2{ (float)VSCREEN_WIDTH / 2 - MeasureTextEx(font,"Press Left Mouse Button to Return to menu", 16, 0).x / 2, VSCREEN_HEIGHT / 2 }, 16, 0, RAYWHITE);
		}
	}
}

void GameScreen::ScreenDeInit() {
	if (currentStage == END_GAME) {
		mainDeck.clearDeck();
		bossDeck.clearDeck();
		playerDeck.clearDeck();
		selectDeck.clearDeck();
		bossBattle.clearDeck();
		playerBattle.clearDeck();
		UnloadTexture(bg);
		UnloadTexture(winFlash);
		UnloadTexture(loseFlash);
		nextBtn.buttonDeInit();
		selectBtn.buttonDeInit();
	}				   
}					  

int GameScreen::getID() { 
	return 10; 
}

void setRandomCards() {
	for (int i = 0; i < 3; i++) {
		int index = GetRandomValue(0, totaldeck);
		std::cout << index << "\n";
		selectDeck.insertExisitingCard(mainDeck.transferCard(index));
		totaldeck--;
		
		if (i <= 3) {
			deckSetup = false;
		}
	}
}

void setOneRandomCard() {
	int index = GetRandomValue(0, totaldeck);
	std::cout << index << "\n";
	selectDeck.insertSelectedExisitingCard(mainDeck.transferCard(index));
	totaldeck--;
}


int resolveBattle(Card* playerCard, Card* bossCard) {
	// Joker interactions
	if (bossCard->face == "Joker") {
		if (playerCard->value == 1 || playerCard->value == 2 || playerCard->value == 3) {
			return 1; // Player wins
		}
		return -1; // Boss wins
	}


	// Same suit: lowest value wins
	if (playerCard->suit == bossCard->suit) {
		if (playerCard->value <= bossCard->value) {
			return 1; // Player wins
		}
		else {
			return -1; // Boss wins
		}
	}

	// Different suits: highest value wins
	if (playerCard->value >= bossCard->value) {
		return 1; // Player wins
	}
	else {
		return -1; // Boss wins
	}
}