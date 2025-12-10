#include "game.h"
#include "collisionData.h"
#include <raymath.h>
#include <random>
#include <iostream>
#include <algorithm>
#include "map.h"
#include "globalVar.h"

Game::Game(): 
isGameOver(false),
mapBoundary1(collisionData, 150, 100, 16, 79732) , 
NPC("resources/image/character/workingman.png", &player, 11, 1, 30), 
arrowProp(arrowLocationData, 150, 100, 16, &arrowPropCondition, MAP_SCALE),
carProp(vehiclePath, 150, 100, 16, &carPropCondition, MAP_SCALE),
characterProp(characterPath, 150, 100, 16, &characterPropCondition, MAP_SCALE),
boatProp(animatedObjectLocation, 150, 100, 16, &animatedObjectPropCondition, MAP_SCALE),
worldDrawProperty(150, 100, &collisionData),
worldHandler("resources/image/office_gang_map.png", "resources/image/office_gang_map_3.png", &worldDrawProperty),
player("resources/image/character/workingman2.png", worldHandler.getWorldCollisionArray(), 3, 50)
{
    // std::cout<<"sss"<< worldHandler.getWorldCollisionArray()->size();
    std::random_device rd;

    // Create a Mersenne Twister generator seeded with rd
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(1.f, 4.0f);
    // Generate a random number
    float randomValue = dis(gen);
    for (int i = 0; i< 5; i++) {
        enemies.emplace_back("resources/image/character/workingman.png", &player, i, dis(gen)/4, dis(gen) + 20);
    }
    worldHandler.foreground.setY(100*TILE_SIZE*MAP_SCALE);
    allDrawableObjects.push_back(&worldHandler.background);
    allDrawableObjects.push_back(&worldHandler.foreground);
    allDrawableObjects.push_back(&player);
}
void Game::tick (float deltaTime) {
    BeginDrawing();
    ClearBackground(BLACK);
    if (!isGameOver) {
        Vector2 mapPos = (Vector2Scale(player.getWorldPos(), -1.f));
        std::vector<Drawing*> allDrawableObjects2 = allDrawableObjects;
        for (Drawing &enemy : enemies) {
            allDrawableObjects2.push_back(&enemy);
        }
        std::sort(allDrawableObjects2.begin(), allDrawableObjects2.end(), [](Drawing* a, Drawing* b) {
            return a->getY() < b->getY();
        });
        for (Drawing* obj : allDrawableObjects2) {
            obj->draw(mapPos);
        }
        player.tick(deltaTime);
        player.drawHealth(0, 0);
        boatProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
        for (AIPlayer &enemy : enemies) {
            // enemy.takeDamage(&player, 100, deltaTime);
    
            enemy.AITick(deltaTime, &enemies);
            enemy.drawHealth();
            // player.takeDamage(&enemy, 100, deltaTime );
        }
        if (enemies.size() == 0) {
            player.updatePlayerState(Idle, true);
        }
        enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](AIPlayer &enemy) {
            return enemy.getHealthComponent().currentHealth <= 0;  // remove if health <= 0
        }),
        enemies.end()
        );
        if (player.getHealthComponent().currentHealth <= 0) isGameOver = true;
    } else {
        DrawText("GAME OVER", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 30, WHITE);
    }
    EndDrawing();
}