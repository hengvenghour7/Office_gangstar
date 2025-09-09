#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"
#include "character.h"

using namespace std;


int main () {
    Vector2 mapPos = {0,0};
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Office Gang");
    SetTargetFPS(60);
    
    Player player1("resources/image/character/workingman2.png");
    AIPlayer character2("resources/image/character/workingman.png");
    while (WindowShouldClose() == false){
        float deltaTime = GetFrameTime();
        mapPos = player1.getWorldPos();
        Image mapImg = LoadImage("resources/image/office_gang_map.png");
        Texture2D mapTexture = LoadTextureFromImage(mapImg);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx(mapTexture, mapPos, 0,MAP_SCALE,WHITE);
        player1.updateCharacterProgess(deltaTime);
        character2.setAIPos(mapPos);
        character2.updateCharacterProgess(deltaTime);
        UnloadImage(mapImg);
            // Free the image, texture now lives on GPU
        EndDrawing();
    }

    CloseWindow();
}
