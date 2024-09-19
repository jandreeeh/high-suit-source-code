#include <iostream>
#include <string>

#include "raylib.h"
#include "reasing.h"
#include "deck.h"
#include "game.h"

//using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define VSCREEN_WIDTH 320
#define VSCREEN_HEIGHT 180

#define MAX(a, b) ((a)>(b)? (a) : (b)) //Used for virtual screen
#define MIN(a, b) ((a)<(b)? (a) : (b))

void AppInit();

static float scale;
static RenderTexture2D target;
static Rectangle virtualSource;
static Rectangle virtualDest;
static Vector2 virtualOrigin;

Deck deck;
float dt = 0;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "High Suit");
    SetTargetFPS(60);
    AppInit();
    GM.SetCurrentScreen(mainMenuScreen);
    GM.GameInit();
 
    while (!WindowShouldClose())
    {
        dt = GetFrameTime();

        GM.GameUpdate();
        if (IsKeyPressed(KEY_ENTER)) {
            GM.TransitionScreen(gameScreen);
        }
        else if(IsKeyPressed(KEY_BACKSPACE)) {
            GM.TransitionScreen(mainMenuScreen);
        }

        BeginTextureMode(target);

            ClearBackground(BLACK);
            GM.GameDraw();

        EndTextureMode();

        BeginDrawing();
    
            DrawTexturePro(target.texture, virtualSource, virtualDest, virtualOrigin, 0.0f, WHITE);
            DrawFPS(20, 20);

        EndDrawing();

    }

    //GameDeInitialize();

    CloseWindow();
    return 0;
}

void AppInit() {
    // Initialization for virtual screen. This allows the program to render the game at 480x270 and display at 1280x720
    scale = MIN((float)GetScreenWidth() / VSCREEN_WIDTH, (float)GetScreenHeight() / VSCREEN_HEIGHT);
    std::cout << scale << "\n";
    target = LoadRenderTexture(VSCREEN_WIDTH, VSCREEN_HEIGHT);

    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    virtualSource = { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height };
    virtualDest = { 0, 0, (float)VSCREEN_WIDTH * scale, (float)VSCREEN_HEIGHT * scale };
    virtualOrigin = { 0, 0 };

    //game.h
    //GameInitialize();
}