#include "game.h"
#include "collisionData.h"
#include <raymath.h>

Game::Game(): mapBoundary1(collisionData, 150, 100, 16, 79732) , player("resources/image/character/workingman2.png", &mapBoundary1)
, NPC("resources/image/character/workingman.png", &player, 11), map(player.getWorldPosPointer(), MAP_SCALE, &collisionData),
arrowProp(arrowLocationData, 150, 100, 16, arrowPropCondition, MAP_SCALE), carProp(vehiclePath, 150, 100, 16, carPropCondition, MAP_SCALE),
characterProp(characterPath, 150, 100, 16, characterPropCondition, MAP_SCALE),
boatProp(animatedObjectLocation, 150, 100, 16, animatedObjectPropCondition, MAP_SCALE)
 {
    for (int i = 0; i< 5; i++) {
        enemies.emplace_back("resources/image/character/workingman.png", &player, i);
    }
}
void Game::tick (float deltaTime) {
    BeginDrawing();
    ClearBackground(BLACK);
    Vector2 mapPos = (Vector2Scale(player.getWorldPos(), -1.f));
    DrawTextureEx(mapTexture, mapPos, 0,MAP_SCALE,WHITE);
    player.tick(deltaTime);
    boatProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
    for (AIPlayer &enemy : enemies) {
        enemy.AITick(deltaTime, &enemies);
        enemy.drawImage();
        enemy.drawHealth();
    }
    EndDrawing();
}