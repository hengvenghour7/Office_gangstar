#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"
#include "character.h"
#include "collisionData.h"
#include "multiPlayer/client.h"

using namespace std;
std::vector<PropDrawCondition> arrowPropCondition {
  {
    79730,
    "resources/image/Modern_UI_Style_1.png",
    8,
    5,
    10,
    0,
    240/15,
    688/21,
  },
};
std::vector<PropDrawCondition> animatedObjectPropCondition {
  {
    79735,
    "resources/image/Fishing_Boat_2_left.png",
    0,
    0,
    8,
    0,
    768/8,
    64,
  },
  {
    79736,
    "resources/image/Fishing_Boat_left_2.png",
    0,
    0,
    8,
    0,
    640/8,
    48,
  },
};
int main () {
    bool isMultiPlayer = false;
    Vector2 mapPos = {0,0};
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Office Gang");
    SetTargetFPS(60);
    std::vector<int> testBound = {};
    MapBoundary mapBoundary1(collisionData, 150, 100, 16, 79732);
    MapProp arrowProp(arrowLocationData, 150, 100, 16, arrowPropCondition, MAP_SCALE);
    MapProp boatProp(animatedObjectLocation, 150, 100, 16, animatedObjectPropCondition, MAP_SCALE);
    Map interiorMap = {
        LoadTexture("resources/image/shop_interior.png")
    };
    Map officeInteriorMap = {
        LoadTexture("resources/image/office_interior.png")
    };
    // Image mapImg = LoadImage("resources/image/office_gang_map.png");
    // Texture2D mapTexture = LoadTextureFromImage(mapImg);
    Player player1("resources/image/character/workingman2.png", &mapBoundary1);
    AIPlayer character2("resources/image/character/workingman.png", &player1);
    MapHandler map1(player1.getWorldPosPointer(), MAP_SCALE, &collisionData);
    if (!isMultiPlayer) {
        while (WindowShouldClose() == false){
            float deltaTime = GetFrameTime();
            mapPos = Vector2Scale(player1.getWorldPos(), -1.f);
            BeginDrawing();
            ClearBackground(BLACK);
            if (IsKeyPressed(KEY_I) && arrowProp.checkInteractionBoundary(player1.getCharacterCollision(), player1.getWorldPos(),0,0, 79740).isCollide) {
                // cout<< "enter available";
                map1.changeMap(interiorMap);
                mapBoundary1.setCollisionData(&shopInteriorCollisionData, 15, 20);
    
        }
            if (IsKeyPressed(KEY_I) && arrowProp.checkInteractionBoundary(player1.getCharacterCollision(), player1.getWorldPos(),0,0, 79738).isCollide) {
                // cout<< "enter available";
                map1.changeMap(officeInteriorMap);
    
        }
            // DrawTextureEx(mapTexture, mapPos, 0,MAP_SCALE,WHITE);
            map1.drawMap(mapPos);
            arrowProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
            boatProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
            // mapBoundary1.drawBoundary(MAP_SCALE, mapPos);
            player1.tick(deltaTime);
            player1.takeDamage(&character2, mapPos, deltaTime);
            player1.drawHealth();
            character2.tick(deltaTime);
            character2.drawHealth();
            character2.takeDamage2(&player1, mapPos, deltaTime);
            // mapBoundary1.drawBoundary(MAP_SCALE, mapPos);
            EndDrawing();
        }
    } else {
        runMultiPlayer();
    }
    CloseWindow();
}
