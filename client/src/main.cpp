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
            game.tick(deltaTime);
        }
    } else {
        runMultiPlayer();
    }
    CloseWindow();
}
