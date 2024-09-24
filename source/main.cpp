#include <iostream>
#include <string>

#include "raylib.h"
#include "reasing.h"
#include "deck.h"
#include "game.h"
#include "globals.h"

//using namespace std;

void AppInit();

static float scale;
static RenderTexture2D target;
static Rectangle virtualSource;
static Rectangle virtualDest;
static Vector2 virtualOrigin;

GameManager GM;
Font font;
float dt = 0;
float mainTime = 0;


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "High Suit");
    SetTargetFPS(60);
    font = LoadFont("source/resources/superstar.ttf");
    AppInit();
    GM.GameInit();
 
    while (!WindowShouldClose())
    {
        dt = GetFrameTime();
        /*mainTime += dt;
        std::cout << "Time: " << mainTime << "\n";*/

        GM.GameUpdate(dt);

        BeginTextureMode(target);

            ClearBackground(BLACK);
            GM.GameDraw();

        EndTextureMode();

        BeginDrawing();
    
            DrawTexturePro(target.texture, virtualSource, virtualDest, virtualOrigin, 0.0f, WHITE);
            DrawFPS(20, 20);

        EndDrawing();

    }


    CloseWindow();
    return 0;
}

void AppInit() {
    scale = MIN((float)GetScreenWidth() / VSCREEN_WIDTH, (float)GetScreenHeight() / VSCREEN_HEIGHT);
    std::cout << scale << "\n";
    target = LoadRenderTexture(VSCREEN_WIDTH, VSCREEN_HEIGHT);

    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    virtualSource = { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height };
    virtualDest = { 0, 0, (float)VSCREEN_WIDTH * scale, (float)VSCREEN_HEIGHT * scale };
    virtualOrigin = { 0, 0 };

}