#pragma once

#include "minigame.h"

#include "raylib.h"
#include <vector>

// Arrow class
class Arrow {
public:
    Vector2 position;
    Vector2 speed;
    bool isReleased;
    Texture2D arrowTexture;

    Arrow(Vector2 pos, Texture2D texture);
    void drawArrow();
    void updateArrow();
    void reset();
};

// Balloon class
class Balloon {
public:
    Vector2 position;
    bool isHit;
    Texture2D balloonTexture;
    float speedY;

    Balloon(Vector2 pos, Texture2D texture);
    void updateBalloon();
    bool checkCollision(Vector2 arrowPos);
};

class MinigameArrow : public Minigame {
public:
    void MinigameInit();
    void MinigameUpdate();
    void MinigameDraw();
    void MinigameDeInit();
};