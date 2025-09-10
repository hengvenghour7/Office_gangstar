#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"
#include "character.h"
#include "collisionData.h"

using namespace std;

int main () {
    Vector2 mapPos = {0,0};
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Office Gang");
    SetTargetFPS(60);
    MapBoundary mapBoundary1(collisionData, 60, 40, 16, 79732);
    MapProp arrowProp(arrowLocationData, 60, 40, 16, 79738);
    Image mapImg = LoadImage("resources/image/office_gang_map.png");
    Texture2D mapTexture = LoadTextureFromImage(mapImg);
    Player player1("resources/image/character/workingman2.png", &mapBoundary1);
    AIPlayer character2("resources/image/character/workingman.png", player1.getWorldPos());
    
    while (WindowShouldClose() == false){
        float deltaTime = GetFrameTime();
        mapPos = Vector2Scale(*player1.getWorldPos(),-1.f) ;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx(mapTexture, mapPos, 0,MAP_SCALE,WHITE);
        arrowProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
        // mapBoundary1.drawBoundary(MAP_SCALE, mapPos);
        player1.tick(deltaTime);
        player1.takeDamage(&character2, mapPos);
        character2.tick(deltaTime);
        EndDrawing();
    }
    CloseWindow();
}
