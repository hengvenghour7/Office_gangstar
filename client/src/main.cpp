#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"
#include "character.h"
#include "collisionData.h"
#include "multiPlayer/client.h"
#include <vector>

using namespace std;
std::vector<PropDrawCondition> arrowPropCondition{
    {79730, "resources/image/Modern_UI_Style_1.png", 8, 5, 10, 0, 240/15, 688/21}
};

std::vector<PropDrawCondition> animatedObjectPropCondition{
    {79735, "resources/image/Fishing_Boat_2_left.png", 0, 0, 8, 0, 768/8, 64},
    {79736, "resources/image/Fishing_Boat_left_2.png", 0, 0, 8, 0, 640/8, 48}
};
std::vector<PropDrawCondition> characterPropCondition{
    {79735, "resources/image/character/woman.png", 0, 8, 4, 0, 896/56, 640/20}
};
std::vector<PropDrawCondition> carPropCondition{
    {79735, "resources/image/Buses_2.png", 0, 1, 3, 0, 2272/15.8, 1200/11}
};
std::vector<PropDrawCondition> interactablePropCondition{
    {79737, "resources/image/Beach_Umbreall_Open.png", 0, 0, 7, 0, 384/8, 304/4.5}
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
    MapProp characterProp(characterPath, 150, 100, 16, characterPropCondition, MAP_SCALE);
    MapProp carProp(vehiclePath, 150, 100, 16, carPropCondition, MAP_SCALE);
    InteractableProp interactableProp(interactableLocation, 150, 100, 16, interactablePropCondition, MAP_SCALE);
    Map interiorMap = {
        LoadTexture("resources/image/shop_interior.png")
    };
    Map officeInteriorMap = {
        LoadTexture("resources/image/office_interior.png")
    };
    Map superMarketMap= {LoadTexture("resources/image/supermarket_map.png")};
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
                mapBoundary1.setCollisionData(&shopInteriorCollisionData, 15, 20, 11256);
    
        }
        if (IsKeyPressed(KEY_I) && arrowProp.checkInteractionBoundary(player1.getCharacterCollision(), player1.getWorldPos(),0,0, 79760).isCollide) {
            // cout<< "enter available";
            map1.changeMap(officeInteriorMap);

    }
            if (IsKeyPressed(KEY_I) && arrowProp.checkInteractionBoundary(player1.getCharacterCollision(), player1.getWorldPos(),0,0, 79730).isCollide) {
                // cout<< "enter available";
                map1.changeMap(superMarketMap);
                mapBoundary1.setCollisionData(&supermarketBoundary, 50, 20, 11256);
    
        }
            // DrawTextureEx(mapTexture, mapPos, 0,MAP_SCALE,WHITE);
            map1.drawMap(mapPos);
            arrowProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
            boatProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
            characterProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
            interactableProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
            carProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
            carProp.moveProps(1, 1);
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
