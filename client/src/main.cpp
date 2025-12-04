#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"
#include "character.h"
#include "collisionData.h"
#include "multiPlayer/client.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "game.h"
#include "globalVar.h"

int main () {
    srand(time(0));
    bool isMultiPlayer = false;
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Office Gang");
    SetTargetFPS(60);
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
