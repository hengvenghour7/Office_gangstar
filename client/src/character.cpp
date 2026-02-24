#include "character.h"
#include "mapCollision.h"
#include "helpers.h"
#include "globalVar.h"

HealthComponent::HealthComponent (float inputMaxHealth) {
    maxHealth = inputMaxHealth;
    currentHealth = maxHealth;
    healthDes = {0, 0};
};
void HealthComponent::takeDamage(float damage) {
    currentHealth-= damage;
};
void HealthComponent::heal(float healAmount) {
    if (currentHealth + healAmount <= maxHealth) {
        currentHealth+= healAmount;
    } else {
        currentHealth = maxHealth;
    };
};
void HealthComponent::drawHealth(float locationX, float locationY, float width, float height, Color inputColor){
    DrawRectangle(locationX, locationY, width, height, inputColor);
};
Character::Character (const char * imageTexture, float speed, float damage, std::vector<std::vector<int>>* worldCollisionArray) : 
            characterHealth(200), speed(speed), takeDamageTimeCap(1.f), stunTimeCap(takeDamageTimeCap + 0.5f), directionState(Right), damage(damage),
            worldCollisionArray(worldCollisionArray) {
            width = 896/56;
            height = 640/20;
            maxCols = 6;
            colIndex = startCols;
            rowIndex = 2;
            characterImg = LoadImage(imageTexture);
            characterTexture = LoadTextureFromImage(characterImg);
            characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            characterCollision = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterHitBox = {screenPos.x + 20, screenPos.y, width*scale_factor, height*scale_factor};
        }
void Character::takeDamage (Character* secondCollider, float damage, float deltaTime) {
            const float TIME_CAP = 2.f;
             if (checkIsCollide(getCharacterCollision(), secondCollider->getCharacterHitBox(), 0, 0).isCollide && secondCollider->getPlayerState2() == Attacking && takeDamageTimeCap >= TIME_CAP) {
                characterHealth.takeDamage(damage);
                takeDamageTimeCap = 0;
                updatePlayerState(Hurt);
                isNeedResetCols = true;
                return;
            }
            takeDamageTimeCap += deltaTime;
            if (takeDamageTimeCap + 1.5 >= TIME_CAP) {
                updatePlayerState(Idle, true);
            }
        }
void Character::updatePlayerState (enum PlayerState state, bool specialUpdate) {
    if (!specialUpdate) {
        if (playerState == Hurt) {
            return;
        }
        playerState2 = state;
        playerState = state;
        return;
    }
    playerState = state;
}
void Character::changeCurrentLevel (int level, int collisionCode, std::vector<std::vector<int>>* collisionArray) {
    currentLevel = level;
    setDrawLevel(level);
    std::cout<< "this level__ " << currentLevel <<std::flush ;
    this->collisionCode = collisionCode;
    worldCollisionArray = collisionArray;
}
int Character::getCurrentLevel () {
    return currentLevel;
}
void Character::setCanSwitchLevel(bool isCanSwitch, int key) {
    canSwitchLevel = {isCanSwitch, key};
}
IsCanSwtichSet Character::getCanSwitchLevel() {
    return canSwitchLevel;
}
void Character::updateDirectionState (Vector2 newDirection) {
    if (newDirection.x > 0) directionState = Right;
    if (newDirection.x < 0) directionState = Left;
    if (newDirection.y > 0) {
        if (newDirection.x == 0) directionState = Down;
    };
    if (newDirection.y < 0) {
        if (newDirection.x == 0) directionState = Up;
    };
    
}
enum PlayerState Character::getPlayerState2 () {
    return playerState2;
}
void Character::updateDirectionStateAI (Vector2 newDirection) {
    if (newDirection.x >= 0) {
        directionState = Right;
    };
    if (newDirection.x < 0) {
        directionState = Left;
    };
    if (newDirection.y >= 0) {
        if (newDirection.x < 0.2 && newDirection.x > -0.2) directionState = Down;
    };
    if (newDirection.y < 0) {
        if (newDirection.x < 0.2 && newDirection.x > -0.2) directionState = Up;
    };
    if (newDirection.x == 0 && direction.y == 0) playerState = Idle;
    if (directionState != oldDirectionState) {
        isNeedResetCols = true;
        oldDirectionState = directionState;
    }
}
void Character::updateAnimation (float deltaTime) {
            updateAnimationTime += deltaTime;
            switch (playerState) {
                case Attacking:
                    switch (directionState)
                    {
                        case Right:
                            rowIndex = 15;
                            startCols = 0;
                            maxCols = 6;
                            break;
                        case Left:
                            rowIndex = 15;
                            startCols = 12;
                            maxCols = 18;
                            break;
                        case Up:
                            rowIndex = 15;
                            startCols = 6;
                            maxCols = 12;
                            break;
                        case Down:
                            rowIndex = 15;
                            startCols = 18;
                            maxCols = 24;
                            break;
                        default:
                            break;
                    }
                    break;
                case Hurt:
                switch (directionState)
                    {
                        case Right:
                            rowIndex = 19;
                            startCols = 0;
                            maxCols = 2;
                            break;
                        case Left:
                            rowIndex = 19;
                            startCols = 6;
                            maxCols = 8;
                            break;
                        case Up:
                            rowIndex = 19;
                            startCols = 3;
                            maxCols = 5;
                            break;
                        case Down:
                            rowIndex = 19;
                            startCols = 9;
                            maxCols = 11;
                            break;
                        default:
                            break;
                    }
                    break;
                case Running:
                    switch (directionState)
                    {
                        case Right:
                            rowIndex = 8;
                            startCols = 0;
                            maxCols = 6;
                            break;
                        case Left:
                            rowIndex = 8;
                            startCols = 12;
                            maxCols = 18;
                            break;
                        case Up:
                            rowIndex = 8;
                            startCols = 6;
                            maxCols = 12;
                            break;
                        case Down:
                            rowIndex = 8;
                            startCols = 18;
                            maxCols = 24;
                            break;
                        default:
                            break;
                    }
                    break;
                case Idle: 
                    switch (directionState)
                    {
                        case Right:
                            rowIndex = 1;
                            startCols = 0;
                            maxCols = 5;
                            break;
                        case Left:
                            rowIndex = 1;
                            startCols = 12;
                            maxCols = 17;
                            break;
                        case Up:
                            rowIndex = 1;
                            startCols = 6;
                            maxCols = 11;
                            break;
                        case Down:
                            rowIndex = 1;
                            startCols = 18;
                            maxCols = 23;
                            break;
                        default:
                            break;
                    }
                    break;
                case Walking:
                    switch (directionState)
                    {
                        case Right:
                        rowIndex = 2;
                        startCols = 0;
                        maxCols = 6;
                        break;
                        case Left:
                            rowIndex = 2;
                            startCols = 12;
                            maxCols = 18;
                            break;
                        case Up:
                            rowIndex = 2;
                            startCols = 6;
                            maxCols = 12;
                            break;
                        case Down:
                            rowIndex = 2;
                            startCols = 18;
                            maxCols = 24;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    switch (directionState)
                    {
                        case Right:
                            rowIndex = 2;
                            startCols = 0;
                            maxCols = 2;
                            break;
                        case Left:
                            rowIndex = 2;
                            startCols = 6;
                            maxCols = 8;
                            break;
                        case Up:
                            rowIndex = 2;
                            startCols = 3;
                            maxCols = 5;
                            break;
                        case Down:
                            rowIndex = 2;
                            startCols = 9;
                            maxCols = 11;
                            break;
                        default:
                            break;
                    }
                    break;
            }
            if (isNeedResetCols) {
                isNeedResetCols = false;
                colIndex = startCols;
            }
            if (updateAnimationTime > 0.1) {
                updateAnimationTime = 0;
                colIndex++;
                if (colIndex > maxCols - 1) colIndex = startCols;
                characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            }
        }
void Character::updateHitBox () {
            characterHitBox.x = characterCollision.x + (directionState == Right ? 20 : directionState == Left ? -20 : 0);
            characterHitBox.y = characterCollision.y + (directionState == Up ? -40 : directionState == Down ? 30 : 0);
            // DrawRectangle(characterHitBox.x, characterHitBox.y, characterHitBox.width, characterHitBox.height, RED);
        };
void Character::setCharacterPos(Vector2 inputWorldPos, Vector2 playerPos) {
        worldPos = inputWorldPos;
        screenPos = Vector2Subtract(worldPos, playerPos);
        characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
        characterCollision = {characterRecDes.x, characterRecDes.y+12, width*scale_factor, (height-6)*scale_factor};
        updateHitBox();
    }
void Character::drawHealth () {
    characterHealth.drawHealth(characterHealth.healthDes.x, characterHealth.healthDes.y, characterHealth.currentHealth, 10, GREEN);
}
void Character::draw (Vector2 des) {
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
        }
Vector2* Character::getWorldPosPointer () {
    return &worldPos;
}
void Character::tick (float deltaTime) {
            updateAnimation(deltaTime);
            // draw({0,0});
        }
Vector2 Character::getCenter (Vector2 mapPos) {
    std::cout<<"this function need to be check later";
    return {0,0};
}
Rectangle Character::getCharacterCollision ()  {
            return characterCollision;
        };
Rectangle Character::getCharacterHitBox ()  {
            return characterHitBox;
        };
HealthComponent* Character::getHealthComponent () {
    return &characterHealth;
}
Player::Player (const char * imageTexture, std::vector<std::vector<int>>* worldCollisionArray, float speed, float damage): Character(imageTexture, speed, damage, worldCollisionArray), 
        healthBarTexture(LoadTexture("resources/image/UI/healthUI.png")),
        coinTexture(LoadTexture("resources/image/UI/coin.png")),
        playerInventory({}) {
            screenPos.x = SCREEN_WIDTH/2;
            screenPos.y = SCREEN_HEIGHT/2;
            worldPos = {200, 100};
            characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterCollision = {screenPos.x, screenPos.y+12, width*scale_factor, (height-6)*scale_factor};
            characterHitBox = {screenPos.x + 30, screenPos.y, width*scale_factor, height*scale_factor};
        }
void Player::setPlayerWorldPos(Vector2 worldPos) {
    this->worldPos = worldPos;
}
void Player::replaceHoldingItems (InteractableItem item) {
    if (holdingItems.size() > 0) {
        holdingItems[0] = item;
    } else {
        holdingItems.push_back(item);
    }
}
Inventory* Player::getPlayerInventory() {
    return &playerInventory;
}
void Player::handleInteraction() {
    std::vector<ShopUIItem>*  inventoryItems = playerInventory.getItems();
    for (size_t i = 0; i < (*inventoryItems).size(); i++) {
        if (checkButtonClick((*inventoryItems)[i].getDimension()).isCollide) {
            getHealthComponent()->heal((*inventoryItems)[i].heal);
            inventoryItems->erase(inventoryItems->begin() + i);
            playerInventory.reArrangeItems();
            break;
        }
    }
}
Vector2 Player::getCenter (Vector2 mapPos) {
    Vector2 center {characterCollision.x + width*scale_factor*0.5f, characterCollision.y + height*scale_factor*0.5f};
    return center;
}
std::vector<InteractableItem>* Player::getHoldingItems() {
    if (holdingItems.size() > 0) {
        Vector2 platerRealLocation = Vector2Add(getWorldPos(), screenPos);
        Vector2 itemDesireLocation = Vector2Add(platerRealLocation, {20, 0});
        holdingItems[0].changeLocation(itemDesireLocation);
    }
    return &holdingItems;
}
void Player::tick (float deltaTime) {
    // DrawRectangle(characterHitBox.x, characterHitBox.y, characterHitBox.width, characterHitBox.height, GREEN);
    if (playerState != Hurt) {
        if (IsKeyDown(KEY_K)) {
                updatePlayerState(Attacking);
            }
        if (IsKeyReleased(KEY_K)) {
            updatePlayerState(Idle);
        }
        if (playerState != Attacking) {
            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_S) ||
                IsKeyReleased(KEY_A) || IsKeyReleased(KEY_W) || IsKeyReleased(KEY_D) || IsKeyReleased(KEY_S)) {
                    isNeedResetCols = true;
                };
            if (IsKeyDown(KEY_A)) {
                direction.x = -1;
            }
            if (IsKeyDown(KEY_D)) {
                direction.x = 1;
            }
            if (IsKeyDown(KEY_W)) {
                direction.y = -1;
            }
            if (IsKeyDown(KEY_S)) {
                direction.y = 1;
            };
            if (direction.x != 0 || direction.y != 0) {
                if (IsKeyDown(KEY_LEFT_SHIFT)) {
                    updatePlayerState(Running);
                } else {
                    updatePlayerState(Walking);
                }
            } else {
                updatePlayerState(Idle);
            }
            updateDirectionState(direction);
            updateHitBox();
        }
        if(Vector2Length(direction) != 0) {
            direction = Vector2Normalize(direction)*speed*deltaTime*50;
            direction.x = (int)(std::round(direction.x)); 
            direction.y = (int)(std::round(direction.y));
            if (!checkPlayerCollisionTile(worldCollisionArray, characterCollision, worldPos, direction, collisionCode).isCollide) {
                worldPos = Vector2Add(worldPos, direction);
                setY(worldPos.y + characterCollision.y + characterCollision.height);
                }
            }
        direction = {0,0};
    } 
        Character::tick(deltaTime);
    }
void Player::draw(Vector2 mapPos) {
    Character::draw(mapPos);
    if (holdingItems.size() > 0) {
        InteractableItem& temp_holdingItem = holdingItems[0];
        DrawTexturePro(*temp_holdingItem.getTexture(), {0,0, (float)temp_holdingItem.getTextureWidth(), (float)temp_holdingItem.getTextureHeight()}, 
        {characterRecDes.x + 50, characterRecDes.y, temp_holdingItem.getDimension().width, temp_holdingItem.getDimension().height},
        {0,0}, 0, WHITE
        );
    }
}
void Player::drawHealth(int x, int y) {
    Vector2 startingPoint = {x - 32.f, y - 5.f};
    float segmentWidth = TILE_SIZE;
    int healthScaleFactor = 3;
    DrawTexturePro(healthBarTexture, {0,0, segmentWidth, TILE_SIZE}, {startingPoint.x, startingPoint.y, segmentWidth*healthScaleFactor, (float)TILE_SIZE*healthScaleFactor}, {0,0}, 0, WHITE);
    DrawTexturePro(healthBarTexture, 
        {segmentWidth,0, segmentWidth, TILE_SIZE}, 
        {startingPoint.x + segmentWidth*healthScaleFactor, startingPoint.y, characterHealth.maxHealth, (float)TILE_SIZE*healthScaleFactor}, 
        {0,0}, 
        0, 
        WHITE);
    DrawTexturePro(healthBarTexture, {segmentWidth*2,0, segmentWidth, TILE_SIZE}, 
        {startingPoint.x + segmentWidth*healthScaleFactor + characterHealth.maxHealth,startingPoint.y, segmentWidth*3, TILE_SIZE*3}, 
        {0,0}, 0, WHITE);
    characterHealth.drawHealth(x + segmentWidth, startingPoint.y + 20, characterHealth.currentHealth, 12, GREEN);
    DrawText((std::to_string(coinAmount)).c_str(), SCREEN_WIDTH - 32*3 - 10, startingPoint.y + 20, 20, WHITE);
    DrawTexturePro(coinTexture, {0,0, 32, 32}, 
        {SCREEN_WIDTH - 32*2, 0, 32*scale_factor, 32*scale_factor}, 
        {0,0}, 0, WHITE);
}
Vector2 Player::getWorldPos () {
            return worldPos;
        }
Vector2 Player::getScreenPos () {
            return screenPos;
        };
void Player::increaseCoin(int amount) {
    coinAmount+= amount;
}
void Player::decreaseCoin(int amount) {
    coinAmount-= amount;
}
int& Player::getCoinAmount() {
    return coinAmount;
}
void Player::changeCollisionCheck (std::vector<std::vector<int>>* newWorldCollisionArray, int newCollisionCode) {
    worldCollisionArray = newWorldCollisionArray;
    collisionCode = newCollisionCode;
}
AIPlayer::AIPlayer (const char * imageTexture, Player* inputPlayer, int id, float speed, float damage, std::vector<std::vector<int>>* worldCollisionArray): 
    Character(imageTexture, speed, damage, worldCollisionArray), id(id) {
    player = inputPlayer;
}
void AIPlayer::AITick(float deltaTime, std::vector<AIPlayer>* allAIPlayer) {
    appraochTarget(allAIPlayer, deltaTime);
    Character::tick(deltaTime);
}
void AIPlayer::drawHealth() {
    characterHealth.healthDes = {characterCollision.x, characterCollision.y};
    characterHealth.drawHealth(characterHealth.healthDes.x, characterHealth.healthDes.y - 20, characterHealth.currentHealth/4, 10, RED);
}
void AIPlayer::doDamage() {
    updatePlayerState(Attacking);
}
void AIPlayer::draw(Vector2 mapPos) {
    Character::draw(mapPos);
    drawHealth();
}
void AIPlayer::appraochTarget (std::vector<AIPlayer>* allAIPlayer, float deltaTime) {
    if (checkIsCollide(characterHitBox, player->getCharacterCollision(), 0, 0).isCollide) {
        doDamage();
        player->takeDamage(this, this->damage, deltaTime );
    }
    takeDamage(player, player->getDamage(), deltaTime);
    if (playerState != Hurt) {
        if (!isNeedToMoveBack) {
            direction = Vector2Normalize(Vector2Subtract(player->getScreenPos(), screenPos));
            for (AIPlayer &enemy : *allAIPlayer) {
                if (enemy.id != id) {
                    CollisionProperty collisionProperty = checkIsCollide(characterCollision, enemy.getCharacterCollision(), 0, 0);
                    if (collisionProperty.isCollide) {
                        if (characterCollision.x < collisionProperty.collider.x) direction.x = -1;
                        if (characterCollision.x >= collisionProperty.collider.x) direction.x = 1;
                        if (characterCollision.y < collisionProperty.collider.y) direction.y = -1;
                        if (characterCollision.y >= collisionProperty.collider.y) direction.y = 1;
                        direction = Vector2Normalize(direction);
                        isNeedToMoveBack = true;
                        this->collider = collisionProperty.collider;
                    }
                }
                if (isNeedToMoveBack) break;
            }
            } else {
                if (std::abs(characterCollision.x - collider.x) > 50) {
                    isNeedToMoveBack = false;
                }
            }
            updatePlayerState(Walking);
            updateDirectionStateAI(direction);
            worldPos = Vector2Add(worldPos,direction*speed);
    }
    setCharacterPos(worldPos, player->getWorldPos());
}