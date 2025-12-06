#include <iostream>
#include <raylib.h>
#include "multiPlayer/client.h"
#include "game.h"
#include "globalVar.h"

int main () {
    bool isMultiPlayer = false;
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Office Gang");
    SetTargetFPS(60);
    Texture2D mapTexture = LoadTexture("resources/image/office_gang_map.png");
    Vector2 mapPos = {0,0};
    Game game;
    if (!isMultiPlayer) {
        while (WindowShouldClose() == false){
            float deltaTime = GetFrameTime();
            // BeginDrawing();
            // ClearBackground(BLACK);
            // Vector2 direction{0,0};
            // if (IsKeyDown(KEY_A)) {
            //     direction.x = -1;
            // }
            // if (IsKeyDown(KEY_D)) {
            //     direction.x = 1;
            // }
            // if (IsKeyDown(KEY_S)) direction.y = 1;
            // if (IsKeyDown(KEY_W)) direction.y = -1;
            // direction = Vector2Normalize(direction)* deltaTime* 50;
            // mapPos = Vector2Add(mapPos, direction);
            // mapPos.x = (int)(std::round(mapPos.x));
            // mapPos.y = (int)(std::round(mapPos.y));
            // DrawTextureEx(mapTexture, mapPos, 0,MAP_SCALE,WHITE);
            // EndDrawing();
            game.tick(deltaTime);
        }
    } else {
        runMultiPlayer();
    }
    CloseWindow();
}
