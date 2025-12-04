#include "character.h"
#include "mapCollision.h"
#include "helpers.h"

HealthComponent::HealthComponent (float inputMaxHealth) {
    maxHealth = inputMaxHealth;
    currentHealth = maxHealth;
    healthDes = {0, 0};
};
void HealthComponent::takeDamage(float damage) {
    currentHealth-= damage;
};
void HealthComponent::heal(float healAmount) {
    currentHealth+= healAmount;
};
void HealthComponent::drawHealth(float locationX, float locationY, float width, float height, Color inputColor){
    DrawRectangle(locationX, locationY, width, height, inputColor);
};
Character::Character (const char * imageTexture) : characterHealth(300) {
            width = 896/56;
            height = 640/20;
            maxCols = 6;
            colIndex = startCols;
            rowIndex = 2;
            speed = 2;
            characterImg = LoadImage(imageTexture);
            characterTexture = LoadTextureFromImage(characterImg);
            characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            characterCollision = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterHitBox = {screenPos.x + 20, screenPos.y, width*scale_factor, height*scale_factor};
        }
void Character::takeDamage (Character* secondCollider, Vector2 MapPos, float deltaTime) {
            if (checkIsCollide(getCharacterCollision(), secondCollider->getCharacterCollision(), MapPos, 0, 0).isCollide && takeDamageTimeCap >= 1.5f) {
                characterHealth.takeDamage(20);
                takeDamageTimeCap = 0;
                takeDamageAnimationTime = 0;
                isTakeDamage = true;
                return;
            }
            takeDamageTimeCap += deltaTime;
            takeDamageAnimationTime += deltaTime;
            if (takeDamageAnimationTime > 0.5) {
                isTakeDamage = false;
            }
            if (isTakeDamage) rowIndex = 19;
        }
void Character::takeDamage2 (Character* secondCollider, Vector2 MapPos, float deltaTime) {
            if (checkIsCollide(getCharacterHitBox(), secondCollider->getCharacterHitBox(), MapPos, 0, 0).isCollide && takeDamageTimeCap >= 1.5f && secondCollider->isAttacking) {
                characterHealth.takeDamage(20);
                takeDamageTimeCap = 0;
                takeDamageAnimationTime = 0;
                isTakeDamage = true;
                return;
            }
            takeDamageTimeCap += deltaTime;
            takeDamageAnimationTime += deltaTime;
            if (takeDamageAnimationTime > 0.5) {
                isTakeDamage = false;
            }
            if (isTakeDamage) {
                rowIndex= 19;
            } 
            else {
                rowIndex=2;
            }
        }
void Character::updateAnimation (float deltaTime) {
            updateAnimationTime += deltaTime;
            if (playerState != Idle) {
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
            } else {
                switch (directionState)
                {
                case Right:
                    rowIndex = 1;
                    startCols = 0;
                    maxCols = 6;
                    break;
                case Left:
                    rowIndex = 1;
                    startCols = 12;
                    maxCols = 18;
                    break;
                case Up:
                    rowIndex = 1;
                    startCols = 6;
                    maxCols = 12;
                    break;
                case Down:
                    rowIndex = 1;
                    startCols = 18;
                    maxCols = 24;
                    break;
                default:
                    break;
                }
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
void Character::setCharacterPos(Vector2 inputWorldPos, Vector2 playerPos) {
        worldPos = inputWorldPos;
        screenPos = Vector2Subtract(worldPos, playerPos);
        characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
        characterCollision = {characterRecDes.x, characterRecDes.y+12, width*scale_factor, (height-6)*scale_factor};
        characterHitBox = {screenPos.x + 20, screenPos.y, width*scale_factor, height*scale_factor};
    }
void Character::drawHealth () {
    characterHealth.drawHealth(characterHealth.healthDes.x, characterHealth.healthDes.y, characterHealth.currentHealth, 10, GREEN);
}
void Character::drawImage () {
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
        }
Vector2* Character::getWorldPosPointer () {
    return &worldPos;
}
void Character::updateDirectionState (Vector2 newDirection) {
    if (newDirection.x >= 0) {
        playerState = Running;
        directionState = Right;
    };
    if (newDirection.x < 0) {
        playerState = Running;
        directionState = Left;
    };
    if (newDirection.y >= 0) {
        playerState = Running;
        if (newDirection.x < 0.2) directionState = Down;
    };
    if (newDirection.y < 0) {
        playerState = Running;
        if (newDirection.x < 0.2) directionState = Up;
    };
    if (newDirection.x == 0 && direction.y == 0) playerState = Idle;
    if (directionState != oldDirectionState) {
        isNeedResetCols = true;
        oldDirectionState = directionState;
    }
}
void Character::tick (float deltaTime) {
            updateAnimation(deltaTime);
            drawImage();
        }
        Rectangle Character::getCharacterCollision ()  {
            return characterCollision;
        };
        Rectangle Character::getCharacterHitBox ()  {
            return characterHitBox;
        };
Player::Player (const char * imageTexture, MapBoundary* inputBoundary): Character(imageTexture) {
            boundary = inputBoundary;
            screenPos.x = SCREEN_WIDTH/2;
            screenPos.y = SCREEN_HEIGHT/2;
            worldPos = {-100, 100};
            characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterCollision = {screenPos.x, screenPos.y+12, width*scale_factor, (height-6)*scale_factor};
            characterHitBox = {screenPos.x + 30, screenPos.y, width*scale_factor, height*scale_factor};
        }
void Player::tick (float deltaTime) {
            if (!isTakeDamage) {
                if (direction.x == 0 && direction.y == 0) {
                    isAttacking = false;
                }
                if (IsKeyDown(KEY_K)) {
                    rowIndex = 13;
                    isAttacking = true;
                }
                else {
                    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_S) ||
                        IsKeyReleased(KEY_A) || IsKeyReleased(KEY_W) || IsKeyReleased(KEY_D) || IsKeyReleased(KEY_S)) isNeedResetCols = true;
                    if (IsKeyDown(KEY_A)) {
                        directionState = Left;
                        direction.x = -1;
                    }
                    if (IsKeyDown(KEY_D)) {
                        directionState = Right;
                        direction.x = 1;
                    }
                    if (IsKeyDown(KEY_W)) {
                        if (direction.x == 0) directionState = Up;
                        direction.y = -1;
                    }
                    if (IsKeyDown(KEY_S)) {
                        if (direction.x == 0) directionState = Down;
                        direction.y = 1;
                    };
                    if (direction.x != 0 || direction.y != 0) rowIndex = 2;
                    isAttacking = false;
                }
                if (direction.x != 0 || direction.y != 0) {
                    playerState = Walking;
                } else {
                    playerState = Idle;
                }
                if(Vector2Length(direction) != 0) {
                        worldPos = Vector2Add(worldPos, Vector2Normalize(direction)*speed);
                        }
                direction = {0,0};
            } 
                Character::tick(deltaTime);
            }
Vector2 Player::getWorldPos () {
            return worldPos;
        }
Vector2 Player::getScreenPos () {
            return screenPos;
        };
AIPlayer::AIPlayer (const char * imageTexture, Player* inputPlayer, int id): Character(imageTexture), id(id) {
    player = inputPlayer;
}
void AIPlayer::AITick(float deltaTime, std::vector<AIPlayer>* allAIPlayer) {
    appraochTarget(allAIPlayer);
    Character::tick(deltaTime);
}
void AIPlayer::drawHealth() {
    characterHealth.healthDes = {characterCollision.x, characterCollision.y};
    characterHealth.drawHealth(characterHealth.healthDes.x, characterHealth.healthDes.y - 20, characterHealth.currentHealth/4, 10, RED);
}
void AIPlayer::appraochTarget (std::vector<AIPlayer>* allAIPlayer) {
    Vector2 direction = Vector2Normalize(Vector2Subtract(player->getScreenPos(), screenPos));
    updateDirectionState(direction);
    for (AIPlayer &enemies : *allAIPlayer) {
        if (enemies.id != id) {
            if (checkIsCollide(getCharacterHitBox(), enemies.getCharacterHitBox(), {0,0}, 0, 0).isCollide) {
                direction.x = -1;
            }
        }
    }
    if(!isTakeDamage) worldPos = Vector2Add(worldPos,direction*AISpeed);
    setCharacterPos(worldPos, player->getWorldPos());
    }