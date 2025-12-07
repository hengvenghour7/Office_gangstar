#include "game.h"
#include "collisionData.h"
#include <raymath.h>
#include <random>
#include <iostream>
#include <algorithm>
#include "map.h"

Game::Game(): 
mapBoundary1(collisionData, 150, 100, 16, 79732) , 
NPC("resources/image/character/workingman.png", &player, 11, 1), 
arrowProp(arrowLocationData, 150, 100, 16, arrowPropCondition, MAP_SCALE),
carProp(vehiclePath, 150, 100, 16, carPropCondition, MAP_SCALE),
characterProp(characterPath, 150, 100, 16, characterPropCondition, MAP_SCALE),
boatProp(animatedObjectLocation, 150, 100, 16, animatedObjectPropCondition, MAP_SCALE),
worldDrawProperty(150, 100, &collisionData),
worldHandler("resources/image/office_gang_map.png", "", &worldDrawProperty),
player("resources/image/character/workingman2.png", worldHandler.getWorldCollisionArray(), 3)
{
    // std::cout<<"sss"<< worldHandler.getWorldCollisionArray()->size();
    std::random_device rd;

    // Create a Mersenne Twister generator seeded with rd
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(1.f, 4.0f);
    // Generate a random number
    float randomValue = dis(gen);
    for (int i = 0; i< 5; i++) {
        enemies.emplace_back("resources/image/character/workingman.png", &player, i, dis(gen)/4);
    }
    allDrawableObjects.push_back(&worldHandler.background);
}
void Game::tick (float deltaTime) {
    BeginDrawing();
    ClearBackground(BLACK);
    Vector2 mapPos = (Vector2Scale(player.getWorldPos(), -1.f));
    // DrawTextureEx(mapTexture, mapPos, 0,MAP_SCALE,WHITE);
    // worldHandler.background.draw(mapPos);
    for (Drawing* obj : allDrawableObjects) {
        obj->draw(mapPos);
    }
    player.tick(deltaTime);
    player.drawHealth(0, 0);
    boatProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
    for (AIPlayer &enemy : enemies) {
        enemy.takeDamage2(&player, mapPos, deltaTime);
        enemy.AITick(deltaTime, &enemies);
        enemy.drawImage();
        enemy.drawHealth();
        player.takeDamage2(&enemy, mapPos, deltaTime );
    }
    enemies.erase(
    std::remove_if(enemies.begin(), enemies.end(), [](AIPlayer &enemy) {
        return enemy.getHealthComponent().currentHealth <= 0;  // remove if health <= 0
    }),
    enemies.end()
    );
    EndDrawing();
}