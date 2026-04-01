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

Time::Time() : countDownTime(0), hour(11), minute(50), dayState(DayState::AM) {

}
void Time::tick(float deltaTime) {
    // std::cout << "count down start " << minute << " _" << std::flush;
    if (countDownTime > 4) {
        countDownTime = 0;
        minute += 10;
        if (minute >= 60) {
            hour += 1;
            minute = 0;
        }
        dayState = hour > 11 ? DayState::PM : DayState::AM;
        if (hour > 23) {
            hour = 0;
        }
    }
    countDownTime += deltaTime;
}
void Time::draw() {
    std::string displayDayState = dayState == DayState::AM ? "AM" : "PM";
    std::string displayTime = (std::to_string(hour % 12 == 0 ? 12 : hour % 12) + " : " + (minute == 0 ? "00" : std::to_string(minute)) + " " + displayDayState);
    DrawText(displayTime.c_str(), SCREEN_WIDTH - 120, 60, 16, WHITE);
}
DayState& Time::getDayState() {
    return dayState;
}
int Time::getHour() {
    return hour;
}
int Time::getMinute() {
    return minute;
}
Game::Game(): 
isGameOver(false),
isMenuOpen(true),
isOpenInteractionDialog(false),
gameState(GameStateEnums::StartScreen),
gameUIState(GameUIStateEnums::Playing),
gameUI(),
shopUI(),
mapBoundary1(collisionData, 150, 100, 16, 79732) , 
NPC("resources/image/character/workingman.png", &player, 11, 1, 30, currentWorld->getWorldCollisionArray()), 
worldDrawProperty(150, 100, &collisionData),
currentWorld(&getCenterWorld(player)),
player("resources/image/character/workingman2.png", currentWorld->getWorldCollisionArray(), 3, 50),
time(),
lightMask(LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT))
{
    std::random_device rd;

    // Create a Mersenne Twister generator seeded with rd
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(1.f, 4.0f);
    // Generate a random number
    float randomValue = dis(gen);
    // for (int i = 0; i< 5; i++) {
    //     enemies.emplace_back("resources/image/character/workingman.png", &player, i, dis(gen)/4, dis(gen) + 20, currentWorld->getWorldCollisionArray());
    // }
    // currentWorld->foreground.setY(100*TILE_SIZE*MAP_SCALE);
    // allDrawableObjects.push_back(&currentWorld->background);
    // allDrawableObjects.push_back(&currentWorld->foreground);
    // allDrawableObjects.push_back(&player);
    // // for (Drawing* propSet : currentWorld->getAllDrawableProps()) {
    // //     allDrawableObjects.push_back(propSet);
    // // }
    // for (Drawing* layer: currentWorld->getMapLayers()) {
    //     allDrawableObjects.push_back(layer);
    // }
    SpawnToDetail spawnToDetail = {WorldEnums::CenterWorld, 0};
    prepareWorld(spawnToDetail);
    

}
void Game::tick (float deltaTime) {
    BeginDrawing();
    ClearBackground(BLACK);
    switch (gameState)
    {
    case GameStateEnums::Playing:
        {
            Vector2 mapPos = (Vector2Scale(player.getWorldPos(), -1.f));
            if (cameraShakeVector.x > 0 || cameraShakeVector.y > 0)
            {
                mapPos = Vector2Add(mapPos, handleCameraShake());
            }
            if (player.getIsSetExplosion())
            {
                player.setExplosion(false);
                cameraShakeVector = {20, 20};
                shakeAmplitudeFactor = 4;
            }
            if (IsKeyPressed(KEY_N))
            {
                player.dash({100, 100}, 20);
            }
            std::vector<Drawing*> allDrawableObjects2 = allDrawableObjects;
            for (Drawing* propSet : currentWorld->getAllDrawableProps()) {
                allDrawableObjects2.push_back(propSet);
            }
            for (Drawing &enemy : enemies) {
                allDrawableObjects2.push_back(&enemy);
            }
            std::sort(allDrawableObjects2.begin(), allDrawableObjects2.end(), [](Drawing* a, Drawing* b) {
                if (a->getDrawLevel() != b->getDrawLevel()) {
                    return a->getDrawLevel() < b->getDrawLevel();
                }
                return a->getY() < b->getY();
            });
            currentWorld->setSwitchersPos(mapPos);
            for (Drawing* obj : allDrawableObjects2) {
                obj->draw(mapPos);
            }
            if (time.getHour() > 15) {
                BeginTextureMode(lightMask);
                ClearBackground(WHITE); 
                float darkIntensity = 0.3 * (time.getHour() - 14);
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, darkIntensity < 1 ? darkIntensity : 1.f));
                if (time.getHour() > 16) {
                    BeginBlendMode(BLEND_ALPHA);
                    DrawCircleGradient(
                        player.getCenter(mapPos).x,
                        player.getCenter(mapPos).y,
                        250,
                        Fade(WHITE, 0.5f),
                        BLANK 
                    );
                    EndBlendMode();
                }

                EndTextureMode();
                BeginBlendMode(BLEND_MULTIPLIED);

                DrawTextureRec(
                    lightMask.texture,
                    (Rectangle){0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT},  // flip Y
                    (Vector2){0, 0},
                    WHITE
                );

                EndBlendMode();
            }
            player.tick(deltaTime);
            player.drawHealth(0, 0);
            time.draw();
            time.tick(deltaTime);
            checkShopInteraction(player, mapPos);
            if (IsKeyReleased(KEY_U)) {
                if (gameUIState == GameUIStateEnums::OpenInventory) {
                    gameUIState = GameUIStateEnums::Playing;
                    break;
                }
                gameUIState = GameUIStateEnums::OpenInventory;
            }
            handleGamePlayUIInteraction();
            for (AIPlayer &enemy : enemies) {
                enemy.AITick(deltaTime, &enemies);
                if (enemy.getHealthComponent()->currentHealth <= 0) {
                    player.increaseCoin(50);
                }
            }
            if (enemies.size() == 0) {
                player.updatePlayerState(Idle, true);
            }
            currentWorld->animateWorldProps(deltaTime);
            enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(), [](AIPlayer &enemy) {
                return enemy.getHealthComponent()->currentHealth <= 0;  // remove if health <= 0
            }),
            enemies.end()
            );
            checkSwitchWorldInteraction(deltaTime, mapPos);
            checkPropsInteraction(player, mapPos);
            if (player.getHealthComponent()->currentHealth <= 0) gameState = GameStateEnums::GameOver;
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
        handleStartMenuTick();
        break;
    
    case GameStateEnums::GameOver :
        DrawText("GAME OVER", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 30, WHITE);
        break;
    
    default:
        break;
    }
    EndDrawing();
}
void Game::checkSwitchWorldInteraction(float deltaTime, Vector2& mapPos) {
    for (auto &[key, value] : (*currentWorld->getAutoMapSwitcherList())) {
        // value.draw(mapPos);
        Rectangle temp_dimension = value.getCollision();
        // DrawRectangle(temp_dimension.x, temp_dimension.y, temp_dimension.width, temp_dimension.height, PURPLE);
        if (checkIsCollide(player.getCharacterCollision(), value.getCollision()).isCollide && switchCooldownTime <= 0) {
            switchCooldownTime = 2;
            isCanSwitch = false;
            SpawnToDetail spawnToDetail = value.getSwitchDestination();
            prepareWorld(spawnToDetail);
            return;
        } else {
            if (switchCooldownTime > 0) {
                switchCooldownTime -= deltaTime;
            }
        }
    }
    for (auto &[key, value] : (*currentWorld->getAutoLevelSwitcherList())) {
        // value.draw(mapPos);
        if (checkIsCollide(player.getCharacterCollision(), value.getScreenPosDimension(mapPos)).isCollide && player.getCanSwitchLevel().isCanSwitch) {
            player.setCanSwitchLevel(false, key);
            int switchToLevel;
            if (player.getCurrentLevel() == value.option1) {
                switchToLevel = value.option2;
            } else {
                switchToLevel = value.option1;
            }
            player.changeCurrentLevel(switchToLevel, 
                (*currentWorld->getLevelDataList()).at(switchToLevel).collisionCode,
                &(*currentWorld->getLevelDataList()).at(switchToLevel).collisionArray);
                return;
        }
        if (!checkIsCollide(player.getCharacterCollision(), value.getScreenPosDimension(mapPos)).isCollide && player.getCanSwitchLevel().isCanSwitch == false && player.getCanSwitchLevel().key == key) {
            player.setCanSwitchLevel(true, key);
        }
    }
    if (IsKeyReleased(KEY_I)) {
        for (auto &[key, value] : (*currentWorld->getMapSwitchersList())) {
            if (checkIsCollide(player.getCharacterCollision(), value.getCollision()).isCollide) {
                SpawnToDetail spawnToDetail = value.getSwitchDestination();
                prepareWorld(spawnToDetail);
                return;
            }
        }
    }
}
void Game::prepareWorld (SpawnToDetail& spawnToDetail) {
    if (currentWorld != nullptr && isWorldInitialize) {
        currentWorld->saveAIPlayers(enemies);
    }
    currentWorld = &getWorld(spawnToDetail.targetMap, player);
    isWorldInitialize = true;
    allDrawableObjects = {};
    enemies = *currentWorld->getAIPlayers();
    player.setPlayerWorldPos(currentWorld->getSpawnLocation(spawnToDetail.targetSpawnPoint));
    player.changeCollisionCheck(currentWorld->getWorldCollisionArray(), currentWorld->getCollisionCode());
    allDrawableObjects.push_back(&currentWorld->background);
    allDrawableObjects.push_back(&currentWorld->foreground);
    currentWorld->foreground.setY(100*TILE_SIZE*MAP_SCALE);
    for (Drawing* layer: currentWorld->getMapLayers()) {
        allDrawableObjects.push_back(layer);
    }
    for (InteractablePropV2 &interactableProp : *currentWorld->getInteractableV2List()) {
        Drawing* drawObj = &interactableProp;
        allDrawableObjects.push_back(drawObj);
    }
    for (Car &car : *currentWorld->getCarList()) {
        Drawing* drawObj = &car;
        allDrawableObjects.push_back(drawObj);
    }
    allDrawableObjects.push_back(&player);
}
void Game::checkPropsInteraction(Player& player, Vector2 mapPos) {
    if (isOpenInteractionDialog) {
        DrawTexturePro(speechBackground, {0,0, 320, 96}, {speechLocation.x, speechLocation.y, 320, 96}, {0,0}, 0, WHITE);
        DrawText(interactionText, speechLocation.x + 50, speechLocation.y + 50, 16, GetColor(THEMECOLOR));
    }
    if (IsKeyReleased(KEY_J)) {
        currentWorld->handleItemPickUp(player, mapPos);
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
        for (InteractablePropV2& prop : *currentWorld->getInteractableV2List()) {
            if (checkCircleInteraction (prop.getCenter(mapPos), player.getCenter(mapPos), 100).isCollide) {
                prop.handleInteraction();
            }
        }
    }
}
void Game::checkShopInteraction(Player &player, Vector2 mapPos) {
    if (IsKeyReleased(KEY_I)) {
        if (gameUIState == GameUIStateEnums::OpenShop) {
            gameUIState = GameUIStateEnums::Playing;
            return;
        }
        for (Shop& shop: *currentWorld->getCurrentWorldShops()) {
            Rectangle screenShopDimension = shop.getShopDimension(mapPos);
            if (checkIsCollide(player.getCharacterCollision(), screenShopDimension).isCollide) {
                shopUI.setShopItems(currentWorld->getShopItems(shop.getShopName()));
                gameUIState = GameUIStateEnums::OpenShop;
                return;
            }
        }
    }
}
void Game::handleStartMenuTick () {
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
}
void Game::loadGame () {
    std::ifstream file ("saved/save.json");
    if (!file.is_open()) {
        return;
    }
    json save;
    file >> save;
    WorldEnums loadedWorld = save["world"]["worldName"].get<WorldEnums>();
    Vector2 loadedLocation = {save["player"]["x"].get<float>(), save["player"]["y"].get<float>()};
    loadWorld(loadedWorld, loadedLocation);
    startGame();
}
void Game::loadWorld(WorldEnums targetMap, Vector2 targetLocation) {
    currentWorld->saveAIPlayers(enemies);
    currentWorld = &getWorld(targetMap, player);
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
void Game::handleGamePlayUIInteraction () {
    switch (gameUIState)
    {
    case GameUIStateEnums::OpenShop:
        shopUI.draw();
        shopUI.handleInteraction(player);
        break;
    case GameUIStateEnums::OpenInventory:
        player.getPlayerInventory()->draw();
        player.handleInteraction();
        break;
    default:
        break;
    }
}
Vector2 Game::handleCameraShake() {
    if (cameraShakeVector.x > 0)
    {
        cameraShakeVector.x-= 0.5;
    }
    if (cameraShakeVector.y > 0)
    {
        cameraShakeVector.y-= 0.5;
    }
    if (shakeAmplitudeFactor > 1)
    {
        shakeAmplitudeFactor -= 0.2;
    }
    Vector2 shakeAmplitude = {sin(cameraShakeVector.y) * shakeAmplitudeFactor, sin(cameraShakeVector.y) * shakeAmplitudeFactor};
    return shakeAmplitude;
}