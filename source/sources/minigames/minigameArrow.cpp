#include "minigames/minigameArrow.h"

#include "globals.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>


Arrow::Arrow(Vector2 pos, Texture2D texture) {
    position = pos;
    speed = { 0.0f, 0.0f };
    isReleased = false;
    arrowTexture = texture;
}

void Arrow::drawArrow() {
    if (isReleased) {
        float angle = Vector2Angle({ 1, 1.5 }, speed) * 3.14;
        DrawTextureEx(arrowTexture, position, angle, 0.9f, WHITE);
    }
}

void Arrow::updateArrow() {
    if (isReleased) {
        speed.y += 18.5f;
        position = Vector2Add(position, Vector2Scale(speed, 0.0009f));

        if (Vector2Length(speed) < 150.0f) {
            speed.y -= 300.3f;
        }

        if (position.x > VSCREEN_WIDTH || position.x < 0 ||
            position.y > VSCREEN_HEIGHT || position.y < 0) {
            reset();
        }
    }
}

void Arrow::reset() {
    position = { 100.0f, 500.0f };
    speed = { 0.0f, 0.0f };
    isReleased = false;
}

Balloon::Balloon(Vector2 pos, Texture2D texture) {
    position = pos;
    isHit = false;
    balloonTexture = texture;
    speedY = -0.5f;
}

void Balloon::updateBalloon() {
    if (!isHit) {
        position.y += speedY;

        if (position.y <= 0) speedY = 0.5f;
        else if (position.y >= VSCREEN_HEIGHT - balloonTexture.height) speedY = -0.5f;

        DrawTexture(balloonTexture, position.x, position.y, WHITE);
    }
}

bool Balloon::checkCollision(Vector2 arrowPos) {
    if (!isHit && CheckCollisionPointCircle(arrowPos, position, 25.0f)) {
        isHit = true;
        return true;
    }
    return false;
}

// Global variables
Texture2D bowTexture, balloonTexture, arrowTexture, groundTexture, backgroundTexture, timerbgTexture;
const int maxBalloons = 5;
Balloon balloons[maxBalloons] = {
    { Vector2{ 100.0f, static_cast<float>(GetRandomValue(10, VSCREEN_WIDTH - 20))}, balloonTexture},
    { Vector2{ 110.0f, static_cast<float>(GetRandomValue(10, VSCREEN_WIDTH - 20)) }, balloonTexture },
    { Vector2{ 120.0f, static_cast<float>(GetRandomValue(10, VSCREEN_WIDTH - 20)) }, balloonTexture },
    { Vector2{ 130.0f, static_cast<float>(GetRandomValue(10, VSCREEN_WIDTH - 20)) }, balloonTexture },
    { Vector2{ 140.0f, static_cast<float>(GetRandomValue(10, VSCREEN_WIDTH - 20)) }, balloonTexture }
};
int maxArrows = 6;
int arrowsFired = 0;
Rectangle ground;
Vector2 archerPos;
std::vector<Arrow> arrows;
int hitCount = 0;
bool gameOver = false;
bool win = false;
bool lastArrowInFlight = false;
int framesCounter = 0;
const int maxTime = 1800;
float shootCooldown = 0.25f;
float timeSinceLastShot = shootCooldown;
bool isDragging = false;
Vector2 initialMousePos;
float dragDistance = 0.0f;

// Game initialization
void MinigameArrow::MinigameInit() {
    SetTargetFPS(60);
    timerbgTexture = LoadTexture("source/resources/sprites/minigame/arrow-parade/timer.png");
    bowTexture = LoadTexture("source/resources/sprites/minigame/arrow-parade/bow.png");
    balloonTexture = LoadTexture("source/resources/sprites/minigame/arrow-parade/balloon.png");
    arrowTexture = LoadTexture("source/resources/sprites/minigame/arrow-parade/arrow.png");
    groundTexture = LoadTexture("source/resources/sprites/minigame/arrow-parade/platform.png");
    backgroundTexture = LoadTexture("source/resources/sprites/minigame/arrow-parade/bg.png");

    ground = { 5.0f, (float)VSCREEN_HEIGHT - groundTexture.height, (float)VSCREEN_WIDTH, (float)groundTexture.height };
    archerPos = { 30.0f, ground.y - bowTexture.height / 2 };

    arrows.push_back(Arrow(archerPos, arrowTexture));
    for (int i = 0; i < 5; i++) {
        balloons[i] = Balloon({ 110.0f + (i * 40), static_cast<float>(GetRandomValue(10, VSCREEN_WIDTH - 20)) }, balloonTexture);
    }

    gameOver = false;
    win = false;
}


void MinigameArrow::MinigameUpdate() {
    if (gameOver || win) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            //gameOver = false;
            //win = false;
            hitCount = 0;
            framesCounter = 0;
            lastArrowInFlight = false;
            arrows.clear();
            arrowsFired = 0;  // Reset arrows fired
            //arrows.push_back(Arrow(archerPos, arrowTexture));
            for (int i = 0; i < 5; i++) {
                //balloons[i].position = { 110.0f + (i * 40), static_cast<float>(GetRandomValue(10, VSCREEN_WIDTH - 20)) };
                balloons[i].isHit = false;
            }
            MinigameHandler.setMinigameDone(true);
        }
    }
    else {
        framesCounter++;
        timeSinceLastShot += GetFrameTime();
        //Drag and shot tech
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && timeSinceLastShot >= shootCooldown && arrowsFired < maxArrows) {
            initialMousePos = GetMousePosition();
            isDragging = true;
        }

        if (isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            Vector2 finalMousePos = GetMousePosition();
            dragDistance = Vector2Distance(initialMousePos, finalMousePos);
            if (dragDistance > 5.0f) {
                Vector2 dragVector = Vector2Subtract({ (float)GetScreenWidth(), (float)GetScreenHeight() / 2 }, finalMousePos);
                float powerFactor = dragDistance * 0.004f;
                arrows.back().speed = Vector2Scale(dragVector, powerFactor);
                arrows.back().isReleased = true;
                timeSinceLastShot = 0.0f;
                arrowsFired++;
                if (arrowsFired < maxArrows) {
                    arrows.push_back(Arrow(archerPos, arrowTexture));
                }
                else {
                    lastArrowInFlight = true;  // The last arrow has been fired
                }
            }
            isDragging = false;
        }

        for (auto& arrow : arrows) arrow.updateArrow();
        for (int i = 0; i < 5; i++) balloons[i].updateBalloon();

        for (int i = 0; i < 5; i++) {
            for (auto& arrow : arrows) {
                if (balloons[i].checkCollision(arrow.position)) {
                    hitCount++;
                    arrow.reset();
                }
            }
        }

        // Check if the game should end
        if (hitCount >= 5) {
            win = true;
            MinigameHandler.setMinigameStats(true);
        }
        else { MinigameHandler.setMinigameStats(false); }
        if (framesCounter >= maxTime || (lastArrowInFlight && !arrows.back().isReleased && hitCount < 5)) {
            gameOver = true;
        }
    }
}


void MinigameArrow::MinigameDraw() {
    //BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTexture(backgroundTexture, 0, 0, WHITE);

    Vector2 bowPos = { archerPos.x - bowTexture.width / 2, archerPos.y - bowTexture.height / 2 };
    DrawTexture(bowTexture, bowPos.x, bowPos.y, WHITE);

    for (auto& arrow : arrows) arrow.drawArrow();
    for (int i = 0; i < 5; i++) balloons[i].updateBalloon();

    DrawTexture(timerbgTexture, VSCREEN_WIDTH - timerbgTexture.width - 8, 5, WHITE); // Draw the texture behind the timer
    DrawTexture(groundTexture, ground.x, ground.y, WHITE);

    DrawTextEx(font, TextFormat("00:%02i", (maxTime - framesCounter) / 60), Vector2{ ((float)VSCREEN_WIDTH - timerbgTexture.width - 5) + 12, 10 }, 16, 0, WHITE);
    //DrawText(TextFormat("Hits: %02i", hitCount), 0, 0, 10, BLACK); // Change 0.3 to 10
    DrawTextEx(font, TextFormat("%2i", maxArrows - arrowsFired), Vector2{ 3, 110 }, 48, 0, WHITE);
    DrawTextEx(font, TextFormat("/%2i", maxArrows), Vector2{ 35, 145 }, 16, 0, WHITE);

    //Crosshair 
    if (isDragging) {
        Vector2 direction = Vector2Subtract({ (float)VSCREEN_WIDTH * 8, (float)VSCREEN_HEIGHT }, GetMousePosition());
        direction = Vector2Scale(Vector2Normalize(direction), dragDistance * 0.05f);
        for (int i = 0; i < 10; i++) {
            Vector2 point = Vector2Add(archerPos, Vector2Scale(direction, (float)i * 1.5f));
            DrawCircleV(point, 2.0f, WHITE);
        }
    }

    if (gameOver) {
        DrawRectangle(0, 0, VSCREEN_WIDTH, VSCREEN_HEIGHT, Fade(RED, 0.5f));
        DrawTextEx(font, "YOU LOSE!", Vector2{ ((float)VSCREEN_WIDTH/ 2) - (MeasureTextEx(font,"YOU LOSE!", 32, 0).x / 2), VSCREEN_HEIGHT / 2 - 30 }, 32, 0, RED);
        DrawTextEx(font, "Press Left Mouse Button to Return", Vector2{ (float)VSCREEN_WIDTH / 2 - MeasureTextEx(font,"Press Left Mouse Button to Return", 16, 0).x / 2, VSCREEN_HEIGHT / 2 }, 16, 0, RAYWHITE); // Change 20 to 10
    }
    else if (win) {
        DrawRectangle(0, 0, VSCREEN_WIDTH, VSCREEN_HEIGHT, Fade(DARKGREEN, 0.5f));

        DrawTextEx(font, "YOU WIN!", Vector2{ (float)VSCREEN_WIDTH / 2 - MeasureTextEx(font,"YOU LOSE!", 32, 0).x / 2, VSCREEN_HEIGHT / 2 - 30 }, 32, 0, GREEN);
        DrawTextEx(font, "Press Left Mouse Button to Return", Vector2{ (float)VSCREEN_WIDTH / 2 - MeasureTextEx(font,"Press Left Mouse Button to Return", 16, 0).x / 2, VSCREEN_HEIGHT / 2 }, 16, 0, RAYWHITE); // Change 20 to 10
    }
    // EndDrawing();
}

// Game deinitialization
void MinigameArrow::MinigameDeInit() {
    UnloadTexture(timerbgTexture);
    UnloadTexture(bowTexture);
    UnloadTexture(balloonTexture);
    UnloadTexture(arrowTexture);
    UnloadTexture(groundTexture);
    UnloadTexture(backgroundTexture);
    //CloseWindow();
}