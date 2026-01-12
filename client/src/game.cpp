#include "game.h"
#include "collisionData.h"
#include <raymath.h>
#include <random>
#include <iostream>
#include <algorithm>
#include "map.h"
#include "globalVar.h"
#include "mapData.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Game::Game(): 
isGameOver(false),
isMenuOpen(true),
isOpenInteractionDialog(false),
gameState(GameStateEnums::StartScreen),
gameUI(),
mapBoundary1(collisionData, 150, 100, 16, 79732) , 
NPC("resources/image/character/workingman.png", &player, 11, 1, 30), 
worldDrawProperty(150, 100, &collisionData),
currentWorld(&getCenterWorld()),
player("resources/image/character/workingman2.png", currentWorld->getWorldCollisionArray(), 3, 50)
{
    std::random_device rd;

    // Create a Mersenne Twister generator seeded with rd
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(1.f, 4.0f);
    // Generate a random number
    float randomValue = dis(gen);
    for (int i = 0; i< 5; i++) {
        enemies.emplace_back("resources/image/character/workingman.png", &player, i, dis(gen)/4, dis(gen) + 20);
    }
    currentWorld->foreground.setY(100*TILE_SIZE*MAP_SCALE);
    allDrawableObjects.push_back(&currentWorld->background);
    allDrawableObjects.push_back(&currentWorld->foreground);
    allDrawableObjects.push_back(&player);
    for (Drawing* propSet : currentWorld->getAllDrawableProps()) {
        allDrawableObjects.push_back(propSet);
    }
    // for (auto &[key, value] : (*currentWorld->getMapSwitchersList())) {
    //     allDrawableObjects.push_back(&value);
    // }
}
void Game::tick (float deltaTime) {
    BeginDrawing();
    ClearBackground(BLACK);
    switch (gameState)
    {
    case GameStateEnums::Playing:
        {
            Vector2 mapPos = (Vector2Scale(player.getWorldPos(), -1.f));
            std::vector<Drawing*> allDrawableObjects2 = allDrawableObjects;
            for (Drawing &enemy : enemies) {
                allDrawableObjects2.push_back(&enemy);
            }
            std::sort(allDrawableObjects2.begin(), allDrawableObjects2.end(), [](Drawing* a, Drawing* b) {
                return a->getY() < b->getY();
            });
            currentWorld->setSwitchersPos(mapPos);
            for (Drawing* obj : allDrawableObjects2) {
                obj->draw(mapPos);
            }
            player.tick(deltaTime);
            player.drawHealth(0, 0);
            // boatProp.drawAllProps(MAP_SCALE, mapPos, deltaTime);
            for (AIPlayer &enemy : enemies) {
                // enemy.takeDamage(&player, 100, deltaTime);
        
                enemy.AITick(deltaTime, &enemies);
                // player.takeDamage(&enemy, 100, deltaTime );
            }
            if (enemies.size() == 0) {
                player.updatePlayerState(Idle, true);
            }
            currentWorld->animateWorldProps(deltaTime);
            enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(), [](AIPlayer &enemy) {
                return enemy.getHealthComponent().currentHealth <= 0;  // remove if health <= 0
            }),
            enemies.end()
            );
            checkSwitchWorldInteraction(player);
            checkPropsInteraction(player, mapPos);
            if (player.getHealthComponent().currentHealth <= 0) gameState = GameStateEnums::GameOver;
            if (IsKeyReleased(KEY_P)) {
                gameState = GameStateEnums::Pause;
            }
            if (IsKeyReleased(KEY_M)) {
                gameState = GameStateEnums::StartScreen;
            }
        }
        break;
    case GameStateEnums::Pause:
        // currentWorld->background.draw({0,0});
        DrawText("PAUSE", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 30, WHITE);
        if (IsKeyReleased(KEY_P)) {
                gameState = GameStateEnums::Playing;
        }
        break;
    case GameStateEnums::StartScreen:
        currentWorld->background.draw(pauseScreenWorldPos);
        gameUI.draw();
        for (Button* &button : gameUI.getMenuButton()) {
            if (checkButtonClick (button->getButtonRec()).isCollide) {
                MenuActionEnums actionType = button->getAction();
                switch (actionType)
                {
                case MenuActionEnums::Start:
                    startGame();
                    break;
                case MenuActionEnums::Save:
                    saveGame();
                    break;
                case MenuActionEnums::Load:
                    loadGame();
                    break;
                default:
                    break;
                }
                break;
            }
        }
        break;
    
    case GameStateEnums::GameOver :
        DrawText("GAME OVER", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 30, WHITE);
        break;
    
    default:
        break;
    }
    EndDrawing();
}
void Game::checkSwitchWorldInteraction(Player& player) {
    if (IsKeyReleased(KEY_I)) {
        for (auto &[key, value] : (*currentWorld->getMapSwitchersList())) {
            if (checkIsCollide(player.getCharacterCollision(), value.getCollision()).isCollide) {
                SpawnToDetail spawnToDetail = value.getSwitchDestination();
                currentWorld->saveAIPlayers(enemies);
                currentWorld = &getWorld(spawnToDetail.targetMap);
                allDrawableObjects = {};
                enemies = *currentWorld->getAIPlayers();
                player.setPlayerWorldPos(currentWorld->getSpawnLocation(spawnToDetail.targetSpawnPoint));
                player.changeCollisionCheck(currentWorld->getWorldCollisionArray(), currentWorld->getCollisionCode());
                currentWorld->foreground.setY(100*TILE_SIZE*MAP_SCALE);
                allDrawableObjects.push_back(&currentWorld->background);
                allDrawableObjects.push_back(&currentWorld->foreground);
                allDrawableObjects.push_back(&player);
                for (Drawing* propSet : currentWorld->getAllDrawableProps()) {
                    allDrawableObjects.push_back(propSet);
                }
                // for (auto &[key, value] : (*currentWorld->getMapSwitchersList())) {
                //     allDrawableObjects.push_back(&value);
                // }
                return;
            }
        }
    }
}
void Game::checkPropsInteraction(Player& player, Vector2 mapPos) {
    if (isOpenInteractionDialog) {
        DrawTexturePro(speechBackground, {0,0, 320, 96}, {speechLocation.x, speechLocation.y, 320, 96}, {0,0}, 0, WHITE);
        DrawText(interactionText, speechLocation.x + 50, speechLocation.y + 50, 16, GetColor(THEMECOLOR));
    }
    if (IsKeyReleased(KEY_I)) {
        if (!isOpenInteractionDialog) {
            for (MapProp* mapProp: *(currentWorld->worldProps)) {
                for (Prop &prop : *mapProp->getMapProp()) {
                    if (checkCircleInteraction (prop.getCenter(mapPos), player.getCenter(mapPos), 100).isCollide) {
                        interactionText =  prop.getInteractionSpeech();
                        isOpenInteractionDialog = true;
                        return;
                    }                
                }
            }
        } else {
            isOpenInteractionDialog = false;
        }
    }
}
void Game::handleMenuClick () {

}
void Game::startGame () {
    gameState = GameStateEnums::Playing;
}
void Game::saveGame () {
    json save;
    save["player"]["x"] = player.getWorldPos().x;
    save["player"]["y"] = player.getWorldPos().y;

    save["world"]["worldName"] = currentWorld->getWorldName();
    std::ofstream file("saved/save.json");
    file << save.dump(4);
    std::cout<< "file saved";
}
void Game::loadGame () {
    std::ifstream file ("saved/save.json");
    if (!file.is_open()) {
        std::cout<<"no save";
        return;
    }
    json save;
    file >> save;
    WorldEnums loadedWorld = save["world"]["worldName"].get<WorldEnums>();
    Vector2 loadedLocation = {save["player"]["x"].get<float>(), save["player"]["y"].get<float>()};
    loadWorld(loadedWorld, loadedLocation);
    std::cout << "load save" << " player x" << save["player"]["x"];
    startGame();
}
void Game::loadWorld(WorldEnums targetMap, Vector2 targetLocation) {
    currentWorld->saveAIPlayers(enemies);
    currentWorld = &getWorld(targetMap);
    allDrawableObjects = {};
    enemies = *currentWorld->getAIPlayers();
    player.setPlayerWorldPos(targetLocation);
    player.changeCollisionCheck(currentWorld->getWorldCollisionArray(), currentWorld->getCollisionCode());
    currentWorld->foreground.setY(100*TILE_SIZE*MAP_SCALE);
    allDrawableObjects.push_back(&currentWorld->background);
    allDrawableObjects.push_back(&currentWorld->foreground);
    allDrawableObjects.push_back(&player);
    for (Drawing* propSet : currentWorld->getAllDrawableProps()) {
        allDrawableObjects.push_back(propSet);
    }
}